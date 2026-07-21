#include <gt_internal/ds/queue/queue.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <gt_internal/ds/list/list.h>

struct mock_thread
{
    int thread_id;
    struct gt_list_node link;
};

static void
init_thread(struct mock_thread *thread, int id)
{
    thread->thread_id = id;

    gt_list_node_init(&thread->link);
}

static void
test_queue_init(void)
{
    struct gt_queue queue;

    gt_queue_init(&queue);

    assert(gt_queue_is_empty(&queue));
    assert(gt_queue_peek(&queue) == NULL);
}

static void
test_enqueue_single(void)
{
    struct gt_queue queue;
    struct mock_thread thread;

    gt_queue_init(&queue);
    init_thread(&thread, 1);

    gt_queue_enqueue(&queue, &thread.link);

    assert(!gt_queue_is_empty(&queue));
    assert(gt_queue_peek(&queue) == &thread.link);
}

static void
test_fifo_order(void)
{
    struct gt_queue queue;

    struct mock_thread t1;
    struct mock_thread t2;
    struct mock_thread t3;

    gt_queue_init(&queue);

    init_thread(&t1, 1);
    init_thread(&t2, 2);
    init_thread(&t3, 3);

    gt_queue_enqueue(&queue, &t1.link);
    gt_queue_enqueue(&queue, &t2.link);
    gt_queue_enqueue(&queue, &t3.link);

    const struct gt_list_node *GT_MAYBE_UNUSED node;

    node = gt_queue_dequeue(&queue);
    assert(node == &t1.link);

    node = gt_queue_dequeue(&queue);
    assert(node == &t2.link);

    node = gt_queue_dequeue(&queue);
    assert(node == &t3.link);

    assert(gt_queue_is_empty(&queue));
}

static void
test_peek_does_not_remove(void)
{
    struct gt_queue queue;
    struct mock_thread thread;

    gt_queue_init(&queue);
    init_thread(&thread, 1);

    gt_queue_enqueue(&queue, &thread.link);

    const struct gt_list_node *GT_MAYBE_UNUSED first;
    const struct gt_list_node *GT_MAYBE_UNUSED second;

    first = gt_queue_peek(&queue);
    second = gt_queue_peek(&queue);

    assert(first == &thread.link);
    assert(second == &thread.link);

    assert(!gt_queue_is_empty(&queue));
}

static void
test_dequeue_empty(void)
{
    struct gt_queue queue;

    gt_queue_init(&queue);

    assert(gt_queue_dequeue(&queue) == NULL);
}

static void
test_interleaved_operations(void)
{
    struct gt_queue queue;

    struct mock_thread a;
    struct mock_thread b;
    struct mock_thread c;

    gt_queue_init(&queue);

    init_thread(&a, 1);
    init_thread(&b, 2);
    init_thread(&c, 3);

    gt_queue_enqueue(&queue, &a.link);
    gt_queue_enqueue(&queue, &b.link);

    assert(gt_queue_dequeue(&queue) == &a.link);

    gt_queue_enqueue(&queue, &c.link);

    assert(gt_queue_dequeue(&queue) == &b.link);
    assert(gt_queue_dequeue(&queue) == &c.link);

    assert(gt_queue_is_empty(&queue));
}

int
main(void)
{
    puts("[RUN] ds/queue");

    test_queue_init();
    test_enqueue_single();
    test_fifo_order();
    test_peek_does_not_remove();
    test_dequeue_empty();
    test_interleaved_operations();

    puts("[PASS] ds/queue");
    return 0;
}
