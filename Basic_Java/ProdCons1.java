// Kobe Davis
// Prof. Li
// CS 415P
// 23 January 2019

import java.util.Queue;
import java.util.ArrayDeque;

public class ProdCons1 {
    static Object lock = new Object();
    static Queue<Integer> pcQueue = new ArrayDeque<Integer>(20);
    static Integer capacity = 20;
    static Integer items = 100;

    static Runnable Consumer = new Runnable() {
        public void run() {
            Integer removed = 0;
            System.out.println(Thread.currentThread().getName() + " starting.");
            try {
                while(items > 0 || removed < 100) {
                    synchronized(lock) {
                        while(pcQueue.isEmpty() == true) {
                            lock.wait();
                        }
                        removed = pcQueue.remove();
                        System.out.println("Consumer removed value " + removed + "(Queue size: " + pcQueue.size() + ")");
                        lock.notify();
                    }
                }
            } catch (Exception e) {}
            System.out.println(Thread.currentThread().getName() + " ending.");
        }
	};

	public static void main(String[] args) {
        try {
            Thread cThread = new Thread(Consumer, "Consumer");
            cThread.start();
            System.out.println("Producer starting.");

            while(items > 0) {
				items = items - 1;
                synchronized(lock) {
                    while(isFull(pcQueue) == true) {
                        lock.wait();
                    }
                    pcQueue.add(100 - items);
					System.out.println("Producer added value " + (100 - items) + "(Queue size: " + pcQueue.size() + ")");
                    lock.notify();
                }
            }
            cThread.join();
        } catch(Exception e) {}
		System.out.println("Produced 100 items. Producer ending and joining consumer thread.");
		System.out.println("Producer/Consumer threads succesful. Exiting program...");
	}

    public static Boolean isFull(Queue<Integer> Q) {
        if(Q.size() == capacity) {
            return true;
        }
        return false;
    }
}
