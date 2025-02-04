package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {
    private String[] queue;
    private int head = 0;
    private int tail = 0;
    private int arrSize = 0;
    private int arrCapacity;
    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        arrCapacity = 5;
        queue = new String[arrCapacity];
    }

    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        arrCapacity = capacity;
        queue = new String[arrCapacity];
    }

    @Override
    public int size() {
        return arrSize;
    }

    @Override
    public boolean isEmpty() {
        return arrSize == 0;
    }

    @Override
    public boolean isFull() {
        return arrSize == arrCapacity;
    }

    @Override
    public boolean enqueue(String obj) {
        if ((obj == null) || (isFull())){
            return false;
        }
        queue[tail] = obj;
        tail = (tail+1)%arrCapacity;
        arrSize++;
        return true;
    }

    @Override
    public String dequeue() {
        if (isEmpty()) return null;
        String ret = queue[head];
        head = (head+1)%arrCapacity;
        arrSize--;
        return ret;
    }

    @Override
    public void printAllElements() {
        int i = head;
        while (i != tail){
            System.out.println(queue[i]);
            i = (i+1)%arrCapacity;
        }
    }
}
