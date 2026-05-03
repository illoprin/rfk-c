#ifdef RFKLIB_TEST

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RFKLIB_IMPL
#include "vector.h"

// Test statistics
int tests_passed = 0;
int tests_failed = 0;

#define ASSERT(cond, msg)                  \
  do {                                     \
    if (cond) {                            \
      tests_passed++;                      \
    } else {                               \
      fprintf(stderr, "[FAIL] %s\n", msg); \
      tests_failed++;                      \
    }                                      \
  } while (0)

int main() {
  printf("--- Vector Test ---\n");

  // 1. Basic test: Init and Push
  {
    Vector v;
    vec_init(&v, int);

    for (int i = 0; i < 100; i++) {
      vec_push(&v, i);
    }

    ASSERT(v.Len == 100, "len must be 100");
    ASSERT(vec_get(&v, int, 0) == 0, "elem 0 must be 0");
    ASSERT(vec_get(&v, int, 99) == 99, "elem 99 must be 99");

    vec_destroy(&v);
  }

  // 2. Test: RemoveAt and Pop
  {
    Vector v;
    vec_init(&v, int);
    int vals[] = {10, 20, 30, 40};
    for (int i = 0; i < 4; i++) {
      vec_push(&v, vals[i]);
    }

    vec_remove(&v, 1);  // Remove 20
    ASSERT(v.Len == 3, "Length after removal must be 3");
    ASSERT(
      vec_get(&v, int, 1) == 30,
      "After removing 20, the second element should become 30"
    );

    vec_pop(&v);  // Remove 40
    ASSERT(v.Len == 2, "Length after Pop must be 2");
    ASSERT(
      vec_get(&v, int, 1) == 30,
      "The last element should remain 30"
    );

    vec_destroy(&v);
  }

  // 3. Test: Insert
  {
    Vector v;
    vec_init(&v, int);
    vec_push(&v, 1);
    vec_push(&v, 3);

    vec_insert(&v, 1, 2);  // Insert '2' at index 1

    ASSERT(v.Len == 3, "Length after Insert must be 3");
    ASSERT(
      vec_get(&v, int, 1) == 2,
      "Element at index 1 should be 2"
    );
    ASSERT(
      vec_get(&v, int, 2) == 3,
      "Element 3 should shift to index 2"
    );

    vec_destroy(&v);
  }

  // 4. Test: Append (vector concatenation)
  {
    Vector v1, v2;
    vec_init(&v1, int);
    vec_init(&v2, int);

    for (int i = 0; i < 5; i++) {
      vec_push(&v1, i);  // 0,1,2,3,4
    }
    for (int i = 5; i < 10; i++) {
      vec_push(&v2, i);  // 5,6,7,8,9
    }

    Vec_AppendRaw(&v1, &v2);

    ASSERT(v1.Len == 10, "Length of concatenated vector must be 10");
    ASSERT(vec_get(&v1, int, 9) == 9, "Last element should be 9");

    // Test on Self-Append (if you've implemented it or want to check
    // for crash) Vec_AppendRaw(&v1, &v1);

    vec_destroy(&v1);
    vec_destroy(&v2);
  }

  // 5. STRESS TEST: 1 million elements
  {
    printf("Running stress test (1,000,000 elements)...\n");
    Vector v;
    vec_init(&v, long);

    for (long i = 0; i < 1000000; i++) {
      vec_push(&v, i);
    }
    ASSERT(v.Len == 1000000, "Million elements pushed");
    ASSERT(v._capacity >= 1000000, "Capacity grew appropriately");

    // Heavy operation: insert at the beginning (shift million
    // elements)
    long val = -100;
    vec_insert_raw(&v, 0, &val);
    ASSERT(
      vec_get(&v, long, 0) == -100,
      "Insert at beginning of huge array succeeded"
    );
    ASSERT(vec_get(&v, long, 1) == 0, "First element shifted");

    vec_destroy(&v);
    printf("Stress test completed.\n");
  }

  // Summary
  printf("\n--- RESULTS ---\n");
  printf("Passed: %d\n", tests_passed);
  printf("Failed: %d\n", tests_failed);

  return (tests_failed > 0) ? 1 : 0;
}

#endif