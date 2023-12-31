#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <assert.h>

static unsigned int ERRNO = 0;

typedef void *list_elem_t;
typedef unsigned long long canary_t;

static const list_elem_t INITIAL = nullptr;
static const int INITIAL_START = 0;
static const int NULL_ELEM = 0;
static const int EMPTY = -1;
static const list_elem_t POISON = (list_elem_t)0xDEADBEEF;
static const int INT_POISON = 0xDEADBEEF;
static const canary_t CANARY = 0xAB8EACAAAB8EACAA;
static const int CANARY_NUM = 2;
static const int MAX_CAPACITY = 100000;

static FILE *list_log = fopen ("list_log.html", "w");

enum errors
{
    LOG_FOPEN_FAIL              = 0x1 << 0,
    LIST_ALLOCATION_FAIL        = 0x1 << 1,
    LIST_BAD_READ_LIST          = 0x1 << 2,
    LIST_BAD_READ_DATA          = 0x1 << 3,
    LIST_INCORRECT_SIZE         = 0x1 << 4,
    LIST_INCORRECT_CAPACITY     = 0x1 << 5,
    LIST_INSERT_ERROR           = 0x1 << 6,
    LIST_INCORRECT_INSERT_PLACE = 0x1 << 7,
    LIST_INCORRECT_REMOVE_PLACE = 0x1 << 8,
    LIST_REMOVE_FROM_EMPTY_LIST = 0x1 << 9,
    LIST_PREV_NEXT_OP_ERR       = 0x1 << 10,
    LIST_FREE_ELEM_NOT_EMPTY    = 0x1 << 11,
    LIST_VIOLATED_LIST          = 0x1 << 12,
    LIST_VIOLATED_DATA          = 0x1 << 13,
};

struct List_elem
{
    list_elem_t data = nullptr;
    int next = 0;
    int prev = 0;
};

struct List_debug
{
    const char *call_func = nullptr;
    const char *call_file = nullptr;
    const char *func      = nullptr;
    const char *file      = nullptr;
    int   call_line = 0;
};

struct List
{
    #ifdef CANARY_PROT
    canary_t left_canary = CANARY;
    #endif

    int free = 0;
    int size = 0;
    int capacity = 0;

    List_elem *elems = nullptr;

    #ifdef LIST_DEBUG
    List_debug debug_info = {};
    #endif

    #ifdef CANARY_PROT
    canary_t right_canary = CANARY;
    #endif
};
static list_elem_t list_get (const List *list, int remove_place, unsigned int *err = &ERRNO);


static void list_ctor          (List *list, int capacity,                     unsigned int *err = &ERRNO);
static void fill_list          (List *list, int start,                        unsigned int *err);
static void list_dtor          (List *list,                                   unsigned int *err = &ERRNO);
static int  list_insert        (List *list, int put_place, list_elem_t value, unsigned int *err = &ERRNO);
static list_elem_t list_remove (List *list, int remove_place,                 unsigned int *err = &ERRNO);
static int  check_list         (const List *list,                             unsigned int *err);
static void list_dump          (const List *list,                             unsigned int *err);
static void dump_list_members  (const List *list,                             unsigned int *err);
static void dump_elems         (const List *list,                             unsigned int *err);
static void dump_list_errors   (const List *list,                             unsigned int *err);
static void make_graph         (const List *list, FILE *list_graph);
static void list_realloc       (List *list, int previous_capacity,            unsigned int *err = &ERRNO);
static int  linearize_list     (List *list,                                   unsigned int *err = &ERRNO);
static void list_free          (List *list);
static void set_error_bit      (unsigned int *error, int bit);

#ifdef LIST_DEBUG
static void init_debug_info (List *list, const int call_line, const char *call_file, const char *call_func,
                      const char *file, const char *func);
static void debug_list_ctor          (List *list, int capacity, unsigned int *err,
                               const int call_line, const char *call_file, const char *call_func);
static void debug_list_dtor          (List *list, unsigned int *err,
                               const int call_line, const char *call_file, const char *call_func);
static int debug_list_insert         (List *list, int put_place, list_elem_t value, unsigned int *err,
                               const int call_line, const char *call_file, const char *call_func);
static list_elem_t debug_list_remove (List *list, int remove_place, unsigned int *err,
                               const int call_line, const char *call_file, const char *call_func);
static int debug_linearize_list      (List *list, unsigned int *err,
                               const int call_line, const char *call_file, const char *call_func);
#endif

static void set_error_bit (unsigned int *error, int bit)
{
    *error |= bit;
}

static void list_ctor (List *list, int capacity, unsigned int *err)
{
    assert (list);

    if (capacity <= 0 || capacity > MAX_CAPACITY)
    {
        set_error_bit (err, LIST_INCORRECT_CAPACITY);
    }
    else
    {
        list->capacity = capacity;

        list_realloc (list, 0);

        list->size = INITIAL_START;

        list->elems[NULL_ELEM].data = INITIAL;
        list->elems[NULL_ELEM].prev = list->elems[NULL_ELEM].next = INITIAL_START;
        
        list->free = 1;

        fill_list (list, INITIAL_START, err);
    }

    check_list (list, err);
}

static int list_insert (List *list, int put_place, list_elem_t value, unsigned int *err)
{
    assert (list);
    assert (err);

    check_list (list, err);

    list_realloc (list, list->capacity);
    int insert_index = list->free;
    int previous_free = list->free;


    if (put_place > MAX_CAPACITY)
    {
        set_error_bit (err, LIST_INCORRECT_INSERT_PLACE);
        check_list (list, err);

        list->free = previous_free;

        return INT_POISON;
    }
    else if (list->elems[put_place].prev == EMPTY && put_place != NULL_ELEM)
    {
        set_error_bit (err, LIST_INSERT_ERROR);

        check_list (list, err);

        return INT_POISON;
    }

    list->free = list->elems[previous_free].next;

    list->elems[insert_index].data = value;
    list->elems[insert_index].prev = put_place;
    list->elems[insert_index].next = list->elems[put_place].next;

    list->elems[put_place].next = insert_index;
    list->elems[list->elems[insert_index].next].prev = insert_index;

    (list->size)++;

    check_list (list, err);

    return insert_index;
}

// inline function??
static list_elem_t list_remove (List *list, int remove_place, unsigned int *err)
{
// size capacity size_t
    if (!(remove_place > 0 && remove_place < list->capacity) || (list->elems[remove_place].prev == EMPTY))
    {
        fprintf (stderr, "REMOVE_ERROR: incorrect remove place");

        set_error_bit (err, LIST_INCORRECT_REMOVE_PLACE);
        list_dump (list, err);

        return POISON;
    }
    else if (list->size == 0)
    {
        fprintf (stderr, "REMOVE_ERROR: remove from empty list");

        set_error_bit (err, LIST_REMOVE_FROM_EMPTY_LIST);
        list_dump (list, err);

        return POISON;
    }

    check_list (list, err);

    list_elem_t return_value = list->elems[remove_place].data;

    list->elems[list->elems[remove_place].prev].next = list->elems[remove_place].next;
    list->elems[list->elems[remove_place].next].prev = list->elems[remove_place].prev;


    list->elems[remove_place].data = POISON;
    list->elems[remove_place].next = list->free;
    list->elems[remove_place].prev = EMPTY;

    list->free = remove_place;

    (list->size)--;

    check_list (list, err);

    return return_value;
}

static list_elem_t list_get (const List *list, int get_place, unsigned int *err)
{
    if (!(get_place > 0 && get_place < list->capacity) || (list->elems[get_place].prev == EMPTY))
    {
        fprintf (stderr, "GET_ERROR: incorrect get place.\n");

        set_error_bit (err, LIST_INCORRECT_REMOVE_PLACE);
        list_dump (list, err);

        return POISON;
    }
    else if (list->size == 0)
    {
        fprintf (stderr, "GET_ERROR: get from empty list.\n");

        set_error_bit (err, LIST_REMOVE_FROM_EMPTY_LIST);
        list_dump (list, err);

        return POISON;
    }

    check_list (list, err);
    list_elem_t return_value = list->elems[get_place].data;

    return return_value;
}


static void list_realloc (List *list, int previous_capacity, unsigned int *err)
{
    if (!(previous_capacity) || list->size >= list->capacity - 1)
    {
        list->capacity += previous_capacity;

        #ifdef CANARY_PROT
        list->elems = (List_elem *)realloc (list->elems, sizeof (List_elem) * list->capacity + sizeof (CANARY) * CANARY_NUM); //check if nullptr
        assert (list->elems);

        if (!(previous_capacity))
        {
            *(canary_t *)(list->elems) = CANARY;
        }

        list->elems = (List_elem *)((char *)list->elems + sizeof (CANARY));

        *(canary_t *)(list->elems + list->capacity) = CANARY;
        #else
        list->elems = (List_elem *)realloc (list->elems, list->capacity * sizeof (List_elem)); //check if nullptr
        assert (list->elems);
        #endif

        if (!(previous_capacity))
        {
            list->free = 1;
        }
        else
        {
            list->free = previous_capacity;
        }
        fill_list (list, previous_capacity, err);
    }
}

static int linearize_list (List *list, unsigned int *err)
{
    //no additional memory
    List_elem *temp_elems = (List_elem *)calloc (list->capacity, sizeof (List_elem));
    assert (temp_elems);

    if (temp_elems == nullptr)
    {
        fprintf (stderr, "ERROR: allocation failed");
        set_error_bit (err, LIST_ALLOCATION_FAIL);

        return *err;
    }

    int phys_index  = list->elems[NULL_ELEM].next;
    int logic_index = 1;

    while (logic_index <= list->size)
    {
        temp_elems[logic_index].data = list->elems[phys_index].data;
        temp_elems[logic_index].next = logic_index + 1;
        temp_elems[logic_index].prev = logic_index - 1;

        phys_index = list->elems[phys_index].next;
        logic_index++;
    }

    temp_elems[logic_index - 1].next = NULL_ELEM;
    temp_elems[NULL_ELEM].prev = logic_index - 1;
    temp_elems[NULL_ELEM].next = 1;
    temp_elems[NULL_ELEM].data = POISON;

    list->free = logic_index;

    while (logic_index < list->capacity)
    {
        temp_elems[logic_index].data = POISON;
        temp_elems[logic_index].next = logic_index + 1;
        temp_elems[logic_index].prev = EMPTY;

        logic_index++;
    }

    temp_elems[--logic_index].next = NULL_ELEM;

    free (list->elems);
    list->elems = temp_elems;

    check_list (list, err);

    return 0;
}

static void fill_list (List *list, int start, unsigned int *err)
{
    for (int index = start; index < list->capacity; index++)
    {
        list->elems[index].data = POISON;
        list->elems[index].prev = EMPTY;

        if (index == list->capacity - 1)
        {
            list->elems[index].next = NULL_ELEM;
        }
        else
        {
            list->elems[index].next = index + 1;
        }
        if (index == 0)
        {
            list->elems[index].next = NULL_ELEM;
            list->elems[index].prev = NULL_ELEM;
        }
    }
}

static void list_dtor (List *list, unsigned int *err)
{
    if (list != nullptr && list->elems != nullptr)
    {
        list_free (list);
    }
    else
    {
        printf ("\nnothing to dtor\n");
    }
}

static void list_free (List *list)
{
    #ifdef CANARY_PROT
    list->elems = (List_elem *)((char *)list->elems - sizeof (CANARY));
    #endif

    free (list->elems);

    list->elems = nullptr;
    list = nullptr;
}

static int check_list (const List *list, unsigned int *err)
{
    int index = NULL_ELEM;
    int counter = 0;

    do
    {
        // if (is_bad_ptr (list))
        // {
        //     set_error_bit (err, LIST_BAD_READ_LIST);
        //     break;
        // }
        // if (is_bad_ptr (list->elems))
        // {
        //     set_error_bit (err, LIST_BAD_READ_DATA);
        //     break;
        // }
        if (list->size < 0)
        {
            set_error_bit (err, LIST_INCORRECT_SIZE);
        }
        if (list->capacity <= 0)
        {
            set_error_bit (err, LIST_INCORRECT_CAPACITY);
        }

        if (!(*err & LIST_INCORRECT_SIZE) && !(*err & LIST_INCORRECT_CAPACITY))
        {
            while (counter++ <= list->size)
            {
                if (list->elems[list->elems[index].next].prev != index)
                {
                    set_error_bit (err, LIST_PREV_NEXT_OP_ERR);
                }
                index = list->elems[index].next;
            }

            counter = 0;
            index = list->free;

            while (counter++ < list->capacity - list->size - 1)
            {
                if (list->elems[index].prev != EMPTY)
                {
                    set_error_bit (err, LIST_FREE_ELEM_NOT_EMPTY);
                }
            }
        }

        #ifdef CANARY_PROT
        if (list->left_canary != CANARY || list->right_canary != CANARY)
        {
            set_error_bit (err, LIST_VIOLATED_LIST);
        }
        if (   *(canary_t *)((char *)list->elems - sizeof (CANARY)) != CANARY
            || *(canary_t)(list->elems + list->capacity) != CANARY)
        {
            set_error_bit (err, LIST_VIOLATED_DATA);
        }
        #endif
    }while(0);

    if (*err)
    {
        list_dump (list, err);
    }

    return 0;
}

static void list_dump (const List *list, unsigned int *err)
{
    static int PNG_FILE_NUMBER = 1;

    FILE *list_graph = fopen ("list_graph", "w");
    assert (list_graph);

    fprintf (list_log, "<pre>\n");

    fprintf (list_log, "<details>");

    dump_list_members (list, err);

    fprintf (list_log, "</details>\n");

    make_graph (list, list_graph);
    fclose (list_graph);

    char cmd[100] = {};

    sprintf (cmd, "Dot list_graph -T png -o dot%d.png", PNG_FILE_NUMBER);
    printf ("%s", cmd);
    system (cmd);


    fprintf (list_log, "<img src = dot%d.png>\n", PNG_FILE_NUMBER++);

    dump_list_errors  (list, err);

    fprintf (list_log, "\n\n\n\n\n");
}

static void make_graph (const List *list, FILE *list_graph)
{
    int idx = 0;
    fprintf (list_graph, "digraph {\n\tgraph [dpi = 100]\n\t"
                         "splines = \"ortho\";\n\t"
                         "rankdir = LR;\n\t"
                         "header [shape = record,  style = \"filled\", fillcolor = \"grey73\","
                         "label = \"idx\\n | data \\n | next \\n | prev \"];\n");
    while (idx < list->capacity)
    {
        if (list->elems[idx].prev == -1)
        {
            fprintf (list_graph, "\tlabel_%d [shape = record, style = \"filled\", fillcolor = \"salmon1\","
                                 "label = \"%d\\n | d[%p]\\n | n[%d]\\n | p[%d](empty)\"];\n ",
                                 idx, idx, list->elems[idx].data, list->elems[idx].next, list->elems[idx].prev);
        }
        else
        {
            fprintf (list_graph, "\tlabel_%d [shape = record, style = \"filled\", fillcolor = \"lightblue\","
                                 "label = \"%d\\n | d[%p]\\n | n[%d]\\n | P [%d]\"];\n ",
                                 idx, idx, list->elems[idx].data, list->elems[idx].next, list->elems[idx].prev);
        }
        idx++;
    }

    idx = 0;
    fprintf (list_graph, "\t{edge [style = \"invis\", arrowsize = 44, weight = 1000];\n\t");

    fprintf (list_graph, "header->label_%d;\n\t", idx);
    while (idx < list->capacity - 1)
    {
        fprintf (list_graph, "label_%d->", idx);
        idx++;
    }
    fprintf (list_graph, "label_%d;\n\t}\n", idx);

    fprintf (list_graph, "\tedge [color = \"purple\", weight = 10];\n\t");
    int counter = 0;
    idx = list->elems[NULL_ELEM].next;

    while (counter++ < list->size)
    {
        fprintf (list_graph, "\tlabel_%d->label_%d;\n", idx, list->elems[idx].next);
        idx = list->elems[idx].next;
    }
    fprintf (list_graph, "}");
}

static void dump_list_members (const List *list, unsigned int *err)
{
    #ifdef LIST_DEBUG
    fprintf (list_log, "%s at file %s, called at %s at %s, line %d\n", list->debug_info.func, list->debug_info.file,
    list->debug_info.call_func, list->debug_info.call_file, list->debug_info.call_line);
    #endif 

    fprintf (list_log, "list[%p]\n", list);

    fprintf (list_log, "list head is %d\nlist tale is %d\n"
                       "list size id %d\nlist capacity id %d\n"
                       "list free is %d\nlist elems is [%p]\n",
                       list->elems[NULL_ELEM].next, list->elems[NULL_ELEM].prev, list->size, list->capacity, list->free, list->elems);
    dump_elems (list, err);
}

static void dump_elems (const List *list, unsigned int *err)
{
    for (int index = 0; index < list->capacity; index++)
    {
        fprintf (list_log, "idx[%d]\t data [%p]\t next is [%d]\t prev is [%d]\n",
                 index, list->elems[index].data, list->elems[index].next, list->elems[index].prev);
    }
}

static void dump_list_errors (const List *list, unsigned int *err)
{

#define log_error(__error_bit, __msg)        \
    if (*err & __error_bit)                  \
    {                                        \
        fprintf (list_log, __msg);           \
    }

    do
    {
        if (*err & LOG_FOPEN_FAIL)
        {
            fprintf (stderr, "opening of log file failed");
            break;
        }

        log_error(LIST_ALLOCATION_FAIL, "calloc failed");
        log_error(LIST_BAD_READ_LIST, "list is a bad ptr");
        log_error(LIST_BAD_READ_DATA, "list elems is a bad ptr");
        log_error(LIST_INCORRECT_CAPACITY, "capacity is incorrect (<=0)");
        log_error(LIST_PREV_NEXT_OP_ERR, "next element of previous is not equal to original");
        log_error(LIST_FREE_ELEM_NOT_EMPTY, "free element is not empty");
        log_error(LIST_VIOLATED_LIST, "access rights of list are invaded");
        log_error(LIST_VIOLATED_DATA, "access rights of list data are invaded");

    } while (0);

#undef log_error
}

#ifdef LIST_DEBUG
static void init_debug_info (List *list, const int call_line, const char *call_file, const char *call_func,
                      const char *file, const char *debug_func)
{
    #ifdef LIST_DEBUG
    list->debug_info.call_line = call_line;
    list->debug_info.call_file = call_file;
    list->debug_info.call_func = call_func;

    list->debug_info.file = file;
    list->debug_info.func = debug_func;
    #endif
}


static void debug_list_ctor (List *list, int capacity, unsigned int *err,
                      const int call_line, const char *call_file, const char *call_func)
{
    init_debug_info (list, call_line, call_file, call_func, __FILE__,
                    "static void list_ctor (List *list, int capacity, unsigned int *err = &ERRNO)");

    list_ctor (list, capacity, err);
}

static void debug_list_dtor (List *list, unsigned int *err,
                      const int call_line, const char *call_file, const char *call_func)
{
    init_debug_info (list, call_line, call_file, call_func, __FILE__,
                    "static void list_dtor (List *list, unsigned int *err = &ERRNO)");

    list_dtor (list, err);
}

static int debug_list_insert (List *list, int put_place, list_elem_t value, unsigned int *err,
                       const int call_line, const char *call_file, const char *call_func)
{
    init_debug_info (list, call_line, call_file, call_func, __FILE__,
                    "int list_insert (List *list, int put_place, list_elem_t value, unsigned int *err = &ERRNO)");

    return list_insert (list, put_place, value, err);
}

static list_elem_t debug_list_remove (List *list, int remove_place, unsigned int *err,
                               const int call_line, const char *call_file, const char *call_func)
{
    init_debug_info (list, call_line, call_file, call_func, __FILE__,
                    "list_elem_t list_remove (List *list, int remove_place, unsigned int *err = &ERRNO)");

    return list_remove (list, remove_place, err);
}

static int debug_linearize_list (List *list, unsigned int *err,
                          const int call_line, const char *call_file, const char *call_func)
{
    init_debug_info (list, call_line, call_file, call_func, __FILE__,
                    "int linearize_list (List *list, unsigned int *err = &ERRNO);");

    return linearize_list (list, err);
}
#endif 

#ifdef LIST_DEBUG

#define list_ctor(list, capacity, err)                                                          \
        debug_list_ctor (list, capacity, err, __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define list_dtor(list, err)                                                                    \
        debug_list_dtor (list, err, __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define list_insert(list, put_place, value, err)                                                \
        debug_list_insert (list, put_place, value, err, __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define list_remove(list, remove_place, err)                                                    \
        debug_list_remove (list, remove_place, err, __LINE__, __FILE__, __PRETTY_FUNCTION__)
#define linearize_list(list, err)                                                               \
        debug_linearize_list (list, err, __LINE__, __FILE__, __PRETTY_FUNCTION__)
#endif

#endif /* LIST_H */
