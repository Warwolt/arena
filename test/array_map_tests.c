#include <rktest/rktest.h>

// Write an ArrayMap implementation in the same style as the SparseArray
// We let the user store the data, and operate on it with functions that only deal with bytes, with
// type-aware macro wrappers.
// The ArrayMap is a Map<String, T> implementation with a fixed N capacity of items (i.e. an array).
//
// Interface
//
//   struct ArrayMap<T, N> {
//       const char keys[KEY_MAX][N];
//       T values[N];
//       size_t num_items;
//   }
//
// Methods
//
// - ArrayMap_insert
// - ArrayMap_get
// - ArrayMap_erase

TEST(ArrayMapTests, Hello) {
	FAIL_INFO("Write this test!\n");
}
