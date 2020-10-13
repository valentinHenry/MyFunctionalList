My Functional List
=================

# About
This exercise is an exercise which I am giving to students at the end of the
"piscine" period at EPITA. It will be used during a live coding for the group
6.

This exercise's goal is to show recursive ways to solve problems. Another goal
is to teach them basic functional functions like map, flatmap, reduce, filter
or flatten.

# Given Files
- A Makefile with the rules below:
	- all: compiles "list.c" into a .o;
	- check: launch the criterion test-suite;
- A header containing the struct and all functions to implement with their descriptios;
- A "test.c" file containing a test-suite written using criterion.

# How to do this exercise?
All of the functions in this exercise must be coded using recursion (even though
some of them may not be terminal).

The given Makefile compiles only "list.c". It is up to you to use more files.

You must not have any double free or leaks if the test-suite is run.

The functions below are the one to be coded:
```c
/**
 ** Applies the function <fn> on all elements of the list.
**/
void list_foreach(const struct list *l, void (*fn)(const void *));

/**
 ** Free the list (but not the elements contained in it).
**/
void list_free(struct list *l);

/**
 ** Add an element to the list.
**/
struct list *list_add(struct list *l, void *e);

/**
 ** Find an element in the list. Returns the list beginning by this element.
 ** If the element is not found, returns NULL.
**/
const struct list *list_find(const struct list *l,
                       const void *e,
                       int(*cmp_fn)(const void *left, const void *right));

/**
 ** Applies the function <map_fn> on the list. Returns a new list with the
 ** new values.
**/
struct list *list_map(const struct list *l, void *(map_fn)(const void *));

/**
 ** Copies a list. This function does not copy the list data.
**/
struct list *list_copy(const struct list *l, struct list **tail);

/**
 ** Flatten a list of list (change the depth of a list from n to n - 1).
**/
struct list *list_flatten(const struct list *l);

/**
 ** Applies the function <map_fn> on all elements of the list, then flatten
 ** it. The temporary list must be freed.
**/
struct list *list_flatmap(const struct list *l,
                          void *(*map_fn)(const void *),
                          void (*free_internal_mapped_list_fn)(void *));

/**
 ** Returns a new list with all elements complying with the <predicate_fn>
 ** function.
**/
struct list *list_filter(const struct list *l,
                         int (*predicate_fn)(const void *));

/**
 ** Reduce the list to a single element using the <reduce_fn> function. If the
 ** list is null, <init> must be return.
**/
void *list_reduce(struct list *l,
                  void *init,
                  void *(*reduce_fn)(const void *element, void *accumulator));

#endif /* ! LIST_H */

Press ENTER or type command to continue
#ifndef LIST_H
# define LIST_H
/**
 ** Structure of a list, NULL is considered as an empty list.
**/
struct list
{
   void        *data;
   struct list *next;
};

/**
 ** Applies the function <fn> on all elements of the list.
**/
void list_foreach(const struct list *l, void (*fn)(const void *));

/**
 ** Free the list (but not the elements contained in it).
**/
void list_free(struct list *l);

/**
 ** Add an element to the list.
**/
struct list *list_add(struct list *l, void *e);

/**
 ** Find an element in the list. Returns the list beginning by this element.
 ** If the element is not found, returns NULL.
**/
const struct list *list_find(const struct list *l,
                       const void *e,
                       int(*cmp_fn)(const void *left, const void *right));

/**
 ** Applies the function <map_fn> on the list. Returns a new list with the
 ** new values.
**/
struct list *list_map(const struct list *l, void *(map_fn)(const void *));

/**
 ** Copies a list. This function does not copy the list data.
**/
struct list *list_copy(const struct list *l, struct list **tail);

/**
 ** Flatten a list of list (change the depth of a list from n to n - 1).
**/
struct list *list_flatten(const struct list *l);

/**
 ** Applies the function <map_fn> on all elements of the list, then flatten
 ** it. The temporary list must be freed using the <free_internal_mapped_list_fn>
 ** function.
**/
struct list *list_flatmap(const struct list *l,
                          void *(*map_fn)(const void *),
                          void (*free_internal_mapped_list_fn)(void *));

/**
 ** Returns a new list with all elements complying with the <predicate_fn>
 ** function.
**/
struct list *list_filter(const struct list *l,
                         int (*predicate_fn)(const void *));

/**
 ** Reduce the list to a single element using the <reduce_fn> function. If the
 ** list is null, <init> must be return.
**/
void *list_reduce(struct list *l,
                  void *init,
                  void *(*reduce_fn)(const void *element, void *accumulator));
```

Have fun!
