#ifndef __RFKLIB_VECTOR_H__
#define __RFKLIB_VECTOR_H__

#include <stddef.h>

typedef struct {
  void* Data;
  size_t _elem_size;
  size_t _capacity;
  size_t Len;
} Vector;

void _vec_init(Vector* vec, size_t elemSize);
void vec_destroy(Vector* vec);

void vec_reserve(Vector* vec, size_t newCapacity);

void vec_push_raw(Vector* vec, void* valuePtr);
void* vec_get_raw(Vector* vec, size_t index) __attribute__((warn_unused_result));
void vec_append_raw(Vector* dst, const Vector* src);

void vec_pop(Vector* vec);
void vec_clear(Vector* vec);

void vec_insert_raw(Vector* vec, size_t index, void* valuePtr);
void vec_remove(Vector* vec, size_t index);

#define vec_init(vec, Type) _vec_init(vec, sizeof(Type))

#define vec_push(vec, value) \
    do { \
        __typeof__(value) tmp = value; \
        vec_push_raw((vec), &tmp); \
    } while(0)

#define vec_get(vec, Type, index) \
    (*(Type*)vec_get_raw((vec), (index)))

#define vec_insert(vec, index, value) \
    do { \
        __typeof__(value) tmp = value; \
        vec_insert_raw((vec), (index), &tmp); \
    } while(0)

#endif // __RFKLIB_VECTOR_H__

#ifdef RFKLIB_IMPL

#define VEC_BASE_CAPACITY 4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _vec_init(Vector* vec, size_t elemSize) {
  // allocate some memory
  void* data = malloc(VEC_BASE_CAPACITY * elemSize);
  if (!data) {
    fprintf(stderr, "[Vector] Allocation failed\n");
    exit(EXIT_FAILURE);
  }
  vec->Data = data;
  vec->_capacity = VEC_BASE_CAPACITY;

  vec->Len = 0;
  vec->_elem_size = elemSize;
}

void vec_destroy(Vector* vec) {
  if (vec->Data) {
    free(vec->Data);
  }
  *vec = (Vector){ 0 };
}

size_t getNewVecCapacity(Vector* vec) {
  return (vec->_capacity == 0)
    ? VEC_BASE_CAPACITY
    : vec->_capacity * 2;
}

void vec_reserve(Vector* vec, size_t newCapacity) {
  if (newCapacity <= vec->_capacity) return;

  void* newData = realloc(vec->Data, newCapacity * vec->_elem_size);
  if (!newData) {
    fprintf(stderr, "[Vector] Allocation failed\n");
    exit(EXIT_FAILURE);
  }

  vec->Data = newData;
  vec->_capacity = newCapacity;
}

void vec_push_raw(Vector* vec, void* valuePtr) {
  if (vec->Len >= vec->_capacity) {
    size_t newCap = getNewVecCapacity(vec);
    vec_reserve(vec, newCap);
  }

  void* dst = (char*)vec->Data + (vec->Len * vec->_elem_size);
  memcpy(dst, valuePtr, vec->_elem_size);

  vec->Len++;
}

void* vec_get_raw(Vector* vec, size_t index) {
  if (index >= vec->Len) {
    fprintf(stderr, "[Vector] Index out of bounds (%zu >= %zu)\n", index, vec->Len);
    return NULL;
  }

  return (char*)vec->Data + (index * vec->_elem_size);
}

void vec_pop(Vector* vec) {
  if (vec->Len == 0) {
    fprintf(stderr, "[Vector] Pop from empty vector\n");
    return;
  }
  vec->Len--;
}

void vec_clear(Vector* vec) {
  vec->Len = 0;
}

void vec_insert_raw(Vector* vec, size_t index, void* valuePtr) {

  if (index > vec->Len) {
    fprintf(stderr, "[Vector] Insertion out of bounds not allowed");
    return;
  }

  if (index == vec->Len) {
    vec_push_raw(vec, valuePtr);
    return;
  }

  // allocate
  if (vec->Len >= vec->_capacity) {
    size_t newCap = getNewVecCapacity(vec);
    vec_reserve(vec, newCap);
  }
  vec->Len++;

  // move subsequent bytes
  void* dst = (char*)vec->Data + (index * vec->_elem_size);
  memmove(
    (char*)dst + vec->_elem_size,
    dst,
    (vec->Len - 1 - index) * vec->_elem_size
  );

  // set value
  memcpy(
    dst,
    valuePtr,
    vec->_elem_size
  );
}

void vec_append_raw(Vector* dst, const Vector* src) {
  if (dst == src) {
    fprintf(stderr, "[Vector] Append failed: self append not allowed\n");
    return;
  }

  if (dst->_elem_size != src->_elem_size) {
    fprintf(stderr, "[Vector] Append failed: element size mismatch\n");
    return;
  }

  if (src->Len == 0) return;

  size_t neededCapacity = dst->Len + src->Len;
  if (neededCapacity > dst->_capacity) {
    // allocate memory with a reserve
    size_t newCap = dst->_capacity;
    if (newCap == 0) newCap = VEC_BASE_CAPACITY;

    while (newCap < neededCapacity) {
      newCap *= 2;
    }
    vec_reserve(dst, newCap);
  }

  // take pointer to end of dst data
  void* target = (char*)dst->Data + (dst->Len * dst->_elem_size);

  // copy data
  memcpy(target, src->Data, src->Len * src->_elem_size);

  dst->Len += src->Len;
}

void vec_remove(Vector* vec, size_t index) {
  if (index >= vec->Len) {
    fprintf(stderr, "[Vector] RemoveAt index out of bounds (%zu >= %zu)\n", index, vec->Len);
    return;
  }

  void* dst = (char*)vec->Data + (index * vec->_elem_size);

  memmove(
    dst,
    (char*)dst + vec->_elem_size,
    (vec->Len - index - 1) * vec->_elem_size
  );

  vec->Len--;
}

#endif // RFKLIB_IMPL