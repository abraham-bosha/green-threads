#include <gt_internal/ds/list/list.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <gt_internal/common/compiler.h>

/**
 * @brief Replicates a runtime Thread Control Block payload framework.
 */
struct mock_thread
{
    int thread_id;
    struct gt_list_node link;
};

/**
 * @struct list_test_context
 * @brief Manages isolated state for collection verification blocks.
 */
struct list_test_context
{
    struct gt_list list;
    struct mock_thread t1;
    struct mock_thread t2;
    struct mock_thread t3;
};

/**
 * @brief Asserts the precise topological placement of an active node link.
 */
static void
verify_links(const struct gt_list_node *GT_MAYBE_UNUSED node,
             const struct gt_list_node *GT_MAYBE_UNUSED expected_prev,
             const struct gt_list_node *GT_MAYBE_UNUSED expected_next)
{
    assert(node->prev == expected_prev);
    assert(node->next == expected_next);
}

/**
 * @brief Sets up a clean test context with structured identification indices.
 */
static void
init_context(struct list_test_context *ctx)
{
    gt_list_init(&ctx->list);

    ctx->t1.thread_id = 101;
    ctx->t2.thread_id = 102;
    ctx->t3.thread_id = 103;

    gt_list_node_init(&ctx->t1.link);
    gt_list_node_init(&ctx->t2.link);
    gt_list_node_init(&ctx->t3.link);
}

/**
 * @brief Sequentially appends all three mock tasks into the context queue.
 */
static void
fill_context(struct list_test_context *ctx)
{
    gt_list_push_back(&ctx->list, &ctx->t1.link);
    gt_list_push_back(&ctx->list, &ctx->t2.link);
    gt_list_push_back(&ctx->list, &ctx->t3.link);
}

static void
test_list_init(void)
{
    struct gt_list list;
    gt_list_init(&list);

    assert(list.head.next == &list.head);
    assert(list.head.prev == &list.head);
    assert(gt_list_is_empty(&list) == true);
    assert(gt_list_front(&list) == NULL);
    assert(gt_list_back(&list) == NULL);
}

static void
test_list_node_init(void)
{
    struct gt_list_node node;
    gt_list_node_init(&node);

    assert(node.prev == NULL);
    assert(node.next == NULL);
    assert(!gt_list_node_is_linked(&node));
}

static void
test_push_front(void)
{
    struct list_test_context ctx;
    init_context(&ctx);

    gt_list_push_front(&ctx.list, &ctx.t2.link);
    gt_list_push_front(&ctx.list, &ctx.t1.link);

    verify_links(&ctx.t1.link, &ctx.list.head, &ctx.t2.link);
    verify_links(&ctx.t2.link, &ctx.t1.link, &ctx.list.head);
}

static void
test_push_back(void)
{
    struct list_test_context ctx;
    init_context(&ctx);

    gt_list_push_back(&ctx.list, &ctx.t1.link);
    gt_list_push_back(&ctx.list, &ctx.t2.link);

    verify_links(&ctx.t1.link, &ctx.list.head, &ctx.t2.link);
    verify_links(&ctx.t2.link, &ctx.t1.link, &ctx.list.head);
}

static void
test_insert_before(void)
{
    struct list_test_context ctx;
    init_context(&ctx);

    gt_list_insert_before(&ctx.list.head, &ctx.t2.link);
    gt_list_insert_before(&ctx.t2.link, &ctx.t1.link);

    verify_links(&ctx.t1.link, &ctx.list.head, &ctx.t2.link);
}

static void
test_insert_after(void)
{
    struct list_test_context ctx;
    init_context(&ctx);

    gt_list_insert_after(&ctx.list.head, &ctx.t1.link);
    gt_list_insert_after(&ctx.t1.link, &ctx.t2.link);

    verify_links(&ctx.t1.link, &ctx.list.head, &ctx.t2.link);
}

static void
test_remove(void)
{
    struct list_test_context ctx;
    init_context(&ctx);
    fill_context(&ctx);

    gt_list_remove(&ctx.t2.link);

    assert(!gt_list_node_is_linked(&ctx.t2.link));
    verify_links(&ctx.t1.link, &ctx.list.head, &ctx.t3.link);
    verify_links(&ctx.t3.link, &ctx.t1.link, &ctx.list.head);
}

static void
test_move_front(void)
{
    struct list_test_context ctx;
    init_context(&ctx);
    fill_context(&ctx);

    gt_list_move_front(&ctx.list, &ctx.t3.link);

    verify_links(&ctx.t3.link, &ctx.list.head, &ctx.t1.link);
    verify_links(&ctx.t1.link, &ctx.t3.link, &ctx.t2.link);
}

static void
test_move_back(void)
{
    struct list_test_context ctx;
    init_context(&ctx);
    fill_context(&ctx);

    gt_list_move_back(&ctx.list, &ctx.t1.link);

    verify_links(&ctx.t1.link, &ctx.t3.link, &ctx.list.head);
    verify_links(&ctx.t2.link, &ctx.list.head, &ctx.t3.link);
}

static void
test_iteration_macro(void)
{
    struct list_test_context ctx;
    init_context(&ctx);
    fill_context(&ctx);

    struct gt_list_node *pos;
    int GT_MAYBE_UNUSED expected_id = 101;

    GT_LIST_FOR_EACH(pos, &ctx.list)
    {
        const struct mock_thread *GT_MAYBE_UNUSED task =
            GT_CONTAINER_OF(pos, struct mock_thread, link);
        assert(task->thread_id == expected_id);
        expected_id++;
    }

    assert(expected_id == 104);
}

static void
test_safe_iteration_macro(void)
{
    struct list_test_context ctx;
    init_context(&ctx);
    fill_context(&ctx);

    struct gt_list_node *pos;
    struct gt_list_node *nxt;
    int GT_MAYBE_UNUSED items_processed = 0;

    GT_LIST_FOR_EACH_SAFE(pos, nxt, &ctx.list)
    {
        const struct mock_thread *GT_MAYBE_UNUSED task =
            GT_CONTAINER_OF(pos, struct mock_thread, link);
        if (task->thread_id == 102)
        {
            gt_list_remove(pos);
        }
        items_processed++;
    }

    assert(items_processed == 3);
    assert(gt_list_front(&ctx.list) == &ctx.t1.link);
    assert(gt_list_back(&ctx.list) == &ctx.t3.link);

    verify_links(&ctx.t1.link, &ctx.list.head, &ctx.t3.link);
}

static void
test_entry_iteration_macro(void)
{
    struct list_test_context ctx;
    init_context(&ctx);
    fill_context(&ctx);

    struct mock_thread *cursor;
    int GT_MAYBE_UNUSED match_val = 101;

    GT_LIST_FOR_EACH_ENTRY(cursor, &ctx.list, link)
    {
        assert(cursor->thread_id == match_val);
        match_val++;
    }

    assert(match_val == 104);
}

int
main(void)
{
    puts("[RUN] ds/list");

    test_list_init();
    test_list_node_init();
    test_push_front();
    test_push_back();
    test_insert_before();
    test_insert_after();
    test_remove();
    test_move_front();
    test_move_back();
    test_iteration_macro();
    test_safe_iteration_macro();
    test_entry_iteration_macro();

    puts("[PASS] ds/list");

    return 0;
}
