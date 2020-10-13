#include <criterion/criterion.h>

#include "list.h"

TestSuite(list, .timeout = 15);

Test(list, add_null_list)
{
   int value = 42;
   struct list *l = list_add(NULL, &value);

   cr_assert_not_null(l);
   cr_assert_eq(l->next, NULL);
   cr_assert_eq(l->data, &value);

   list_free(l);
}

Test(list, add_existing_list)
{
   int value1 = 42;
   int value2 = 24;

   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);

   cr_assert_null(l->next);
   cr_assert_eq(l->data, &value1);
   cr_assert_eq(l2->next, l);
   cr_assert_eq(l2->data, &value2);

   list_free(l2);
}

int int_cmp(const void *left, const void* right)
{
   return *(const int *)left - *(const int *)right;
}

Test(list, list_find_NULL)
{
   int value = 5;
   const struct list *l = list_find(NULL, &value, int_cmp);
   cr_assert_null(l);
}

Test(list, list_find_existing_head)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;

   struct list *l1 = list_add(NULL, &value2);
   struct list *l2 = list_add(l1, &value1);
   struct list *l3 = list_add(l2, &value3);

   const struct list *lf = list_find(l3, &value2, int_cmp);

   cr_assert_eq(lf->data, &value2);

   list_free(l3);
}

Test(list, list_find_existing_center)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;

   struct list *l1 = list_add(NULL, &value2);
   struct list *l2 = list_add(l1, &value1);
   struct list *l3 = list_add(l2, &value3);

   const struct list *lf = list_find(l3,&value1, int_cmp);
   cr_assert_eq(lf->data, &value1);

   list_free(l3);
}

Test(list, list_find_existing_tail)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;

   struct list *l1 = list_add(NULL, &value2);
   struct list *l2 = list_add(l1, &value1);
   struct list *l3 = list_add(l2, &value3);

   const struct list *lf = list_find(l3, &value3, int_cmp);
   cr_assert_eq(lf->data, &value3);

   list_free(l3);
}

Test(list, list_find_not_existing)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;
   int value4 = 4;

   struct list *l1 = list_add(NULL, &value2);
   struct list *l2 = list_add(l1, &value1);
   struct list *l3 = list_add(l2, &value3);

   const struct list *lf = list_find(l3, &value4, int_cmp);
   cr_assert_null(lf);

   list_free(l3);
}

void list_free_with_data(struct list *l)
{
   if (!l)
      return;

   list_free_with_data(l->next);
   free(l->data);
   free(l);
}

void *times2(const void *val)
{
   int *res = malloc(sizeof(int));
   *res = *(const int*)val * 2;
   return res;
}

Test(list, list_map_null)
{
   struct list *l = list_map(NULL, times2);
   cr_assert_null(l);
}

Test(list, list_map_one_elt)
{
   int value1 = 1;

   struct list *l = list_add(NULL, &value1);

   struct list *mapped_list = list_map(l, times2);

   cr_assert_eq(*(int*)mapped_list->data, 2);
   list_free(l);
   list_free_with_data(mapped_list);
}

Test(list, list_map)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;

   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);
   struct list *l3 = list_add(l2, &value3);

   struct list *mapped_list = list_map(l3, times2);
   cr_assert_eq(*(int*)mapped_list->data, 6);
   cr_assert_eq(*(int*)mapped_list->next->data, 4);
   cr_assert_eq(*(int*)mapped_list->next->next->data, 2);
   list_free(l3);
   list_free_with_data(mapped_list);
}

Test(list, list_copy_null)
{
   struct list *tail;
   struct list *l = list_copy(NULL, &tail);
   cr_assert_null(l);
   cr_assert_null(tail);
}

Test(list, list_copy)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;

   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);
   struct list *l3 = list_add(l2, &value3);

   struct list *tail;

   struct list *copy = list_copy(l3, &tail);
   cr_assert_not_null(copy);
   cr_assert_eq(copy->data, &value3);
   cr_assert_eq(copy->next->data, &value2);
   cr_assert_eq(copy->next->next->data, &value1);

   cr_assert_eq(copy->next->next, tail);
   cr_assert_eq(tail->data, &value1);


   list_free(l3);
   list_free(copy);
}

Test(list, flatten_null)
{
   struct list *l = list_flatten(NULL);
   cr_assert_null(l);
}

Test(list, flatten_list)
{
   int value0 = 0;
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;
   struct list l3 = {
      .data = &value3,
      .next = NULL
   };
   struct list l2 = {
      .data = &value2,
      .next = &l3
   };

   struct list l1 = {
      .data = &value1,
      .next = NULL
   };

   struct list l0 = {
      .data = &value0,
      .next = NULL
   };

   struct list *la = list_add(NULL, &l0);
   struct list *lb = list_add(la, &l2);
   struct list *lc = list_add(lb, &l1);

   struct list *flattened = list_flatten(lc);

   cr_assert_eq(flattened->data, &value1);
   cr_assert_eq(flattened->next->data, &value2);
   cr_assert_eq(flattened->next->next->data, &value3);
   cr_assert_eq(flattened->next->next->next->data, &value0);

   list_free(lc);
   list_free(flattened);
}

void *duplicate_value(const void *value){
   struct list *l1 = malloc(sizeof(struct list));
   struct list *l2 = malloc(sizeof(struct list));

   l1->next = l2;
   l2->next = NULL;
   l1->data = (void *)value;
   l2->data = (void *)value;

   return l1;
}

void list_free_void(void *list)
{
   list_free((struct list *)list);
}

Test(list, flatmap_null)
{
   struct list *l = list_flatmap(NULL, duplicate_value, list_free_void);
   cr_assert_null(l);
}

Test(list, flatmap_list)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;


   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);
   struct list *l3 = list_add(l2, &value3);

   struct list *map_flat = list_flatmap(l3, duplicate_value, list_free_void);

   cr_assert_eq(map_flat->data, &value3);
   cr_assert_eq(map_flat->next->data, &value3);
   cr_assert_eq(map_flat->next->next->data, &value2);
   cr_assert_eq(map_flat->next->next->next->data, &value2);
   cr_assert_eq(map_flat->next->next->next->next->data, &value1);
   cr_assert_eq(map_flat->next->next->next->next->next->data, &value1);

   list_free(l3);
   list_free(map_flat);
}

int is_odd(const void *value)
{
   return *(int *)value % 2;
}

Test(list, filter_null)
{
   struct list *l = list_filter(NULL, is_odd);
   cr_assert_null(l);
}

Test(list, filter_all_odd)
{
   int value1 = 1;
   int value2 = 3;
   int value3 = 5;

   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);
   struct list *l3 = list_add(l2, &value3);

   struct list *filtered = list_filter(l3, is_odd);

   cr_assert_not_null(filtered);
   cr_assert_eq(filtered->data, &value3);
   cr_assert_eq(filtered->next->data, &value2);
   cr_assert_eq(filtered->next->next->data, &value1);

   list_free(l3);
   list_free(filtered);
}

Test(list, filter_all_even)
{
   int value1 = 2;
   int value2 = 4;
   int value3 = 6;

   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);
   struct list *l3 = list_add(l2, &value3);

   struct list *filtered = list_filter(l3, is_odd);

   cr_assert_null(filtered);

   list_free(l3);
}

Test(list, filter_mixed)
{
   int value1 = 1;
   int value2 = 2;
   int value3 = 3;
   int value4 = 4;

   struct list *l = list_add(NULL, &value1);
   struct list *l2 = list_add(l, &value2);
   struct list *l3 = list_add(l2, &value3);
   struct list *l4 = list_add(l3, &value4);

   struct list *filtered = list_filter(l4, is_odd);

   cr_assert_not_null(filtered);
   cr_assert_eq(filtered->data, &value3);
   cr_assert_eq(filtered->next->data, &value1);

   list_free(l4);
   list_free(filtered);
}

void *add(const void *left, void *right)
{
   int* res = malloc(sizeof(int));
   *res = *(int *)left + *(int *)right;
   free(right);
   return res;
}

Test(list, reduce_null)
{
   int value = 2;
   int *res = list_reduce(NULL, &value, add);
   cr_assert_eq(res, &value);
}

Test(list, reduce_list)
{
   int* value = malloc(sizeof(int));
   *value = 1;

   int value2 = 2;
   int value3 = 3;
   int value4 = 4;

   struct list *l2 = list_add(NULL, &value2);
   struct list *l3 = list_add(l2, &value3);
   struct list *l4 = list_add(l3, &value4);

   int *res = list_reduce(l4, value, add);

   cr_assert_eq(*res, 10);
   free(res);
   list_free(l4);
}

// TODO foreach test

int main(int argc, char *argv[])
{
   struct criterion_test_set *tests = criterion_initialize();

   int result = 0;

   if (criterion_handle_args(argc, argv, true))
      result = !criterion_run_all_tests(tests);

   criterion_finalize(tests);

   return result;
}
