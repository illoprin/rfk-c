#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

typedef struct {
  void* Data;
  size_t _elemSize;
  size_t _capacity;
  size_t Len;
} Vector;

void __vecInit(Vector* vec, size_t elemSize);
void Vec_Destroy(Vector* vec);

void Vec_Reserve(Vector* vec, size_t newCapacity);

void Vec_PushRaw(Vector* vec, void* valuePtr);
void* Vec_AtRaw(Vector* vec, size_t index) __attribute__((warn_unused_result));

void Vec_Pop(Vector* vec);
void Vec_Clear(Vector* vec);

void Vec_InsertRaw(Vector* vec, size_t index, void* valuePtr);
void Vec_RemoveAt(Vector* vec, size_t index);

#define Vec_Init(vec, Type) __vecInit(vec, sizeof(Type))

#define Vec_Push(vec, value) \
    do { \
        __typeof__(value) tmp = value; \
        Vec_PushRaw((vec), &tmp); \
    } while(0)

#define Vec_At(vec, Type, index) \
    (*(Type*)Vec_AtRaw((vec), (index)))

#define Vec_Insert(vec, index, value) \
    do { \
        __typeof__(value) tmp = value; \
        Vec_InsertRaw((vec), (index), &tmp); \
    } while(0)

#ifdef VECTOR_H_IMPLEMENTATION

#define VEC_BASE_CAPACITY 4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void __vecInit(Vector* vec, size_t elemSize) {
  // allocate some memory
  void* data = malloc(VEC_BASE_CAPACITY * elemSize);
  if (!data) {
    fprintf(stderr, "[Vector] Allocation failed\n");
    exit(EXIT_FAILURE);
  }
  vec->Data = data;
  vec->_capacity = VEC_BASE_CAPACITY;

  vec->Len = 0;
  vec->_elemSize = elemSize;
}

void Vec_Destroy(Vector* vec) {
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

void Vec_Reserve(Vector* vec, size_t newCapacity) {
  if (newCapacity <= vec->_capacity) return;

  void* newData = realloc(vec->Data, newCapacity * vec->_elemSize);
  if (!newData) {
    fprintf(stderr, "[Vector] Allocation failed\n");
    exit(EXIT_FAILURE);
  }

  vec->Data = newData;
  vec->_capacity = newCapacity;
}

void Vec_PushRaw(Vector* vec, void* valuePtr) {
  if (vec->Len >= vec->_capacity) {
    size_t newCap = getNewVecCapacity(vec);
    Vec_Reserve(vec, newCap);
  }

  void* dst = (char*)vec->Data + (vec->Len * vec->_elemSize);
  memcpy(dst, valuePtr, vec->_elemSize);

  vec->Len++;
}

void* Vec_AtRaw(Vector* vec, size_t index) {
  if (index >= vec->Len) {
    fprintf(stderr, "[Vector] Index out of bounds (%zu >= %zu)\n", index, vec->Len);
    return NULL;
  }

  return (char*)vec->Data + (index * vec->_elemSize);
}

void Vec_Pop(Vector* vec) {
  if (vec->Len == 0) {
    fprintf(stderr, "[Vector] Pop from empty vector\n");
    return;
  }
  vec->Len--;
}

void Vec_Clear(Vector* vec) {
  vec->Len = 0;
}

void Vec_InsertRaw(Vector* vec, size_t index, void* valuePtr) {

  if (index > vec->Len) {
    fprintf(stderr, "[Vector] Insertion out of bounds not allowed");
    return;
  }

  if (index == vec->Len) {
    Vec_PushRaw(vec, valuePtr);
    return;
  }

  // allocate
  if (vec->Len >= vec->_capacity) {
    size_t newCap = getNewVecCapacity(vec);
    Vec_Reserve(vec, newCap);
  }
  vec->Len++;

  // move subsequent bytes
  void* dst = (char*)vec->Data + (index * vec->_elemSize);
  memmove(
    (char*)dst + vec->_elemSize,
    dst,
    (vec->Len - 1 - index) * vec->_elemSize
  );

  // set value
  memcpy(
    dst,
    valuePtr,
    vec->_elemSize
  );
}

void Vec_AppendRaw(Vector* dst, const Vector* src) {
  if (dst == src) {
    fprintf(stderr, "[Vector] Append failed: self append not allowed\n");
    return;
  }

  if (dst->_elemSize != src->_elemSize) {
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
    Vec_Reserve(dst, newCap);
  }

  // take pointer to end of dst data
  void* target = (char*)dst->Data + (dst->Len * dst->_elemSize);

  // copy data
  memcpy(target, src->Data, src->Len * src->_elemSize);

  dst->Len += src->Len;
}

void Vec_RemoveAt(Vector* vec, size_t index) {
  if (index >= vec->Len) {
    fprintf(stderr, "[Vector] RemoveAt index out of bounds (%zu >= %zu)\n", index, vec->Len);
    return;
  }

  void* dst = (char*)vec->Data + (index * vec->_elemSize);

  memmove(
    dst,
    (char*)dst + vec->_elemSize,
    (vec->Len - index - 1) * vec->_elemSize
  );

  vec->Len--;
}

#endif

#endif // __VECTOR_H__