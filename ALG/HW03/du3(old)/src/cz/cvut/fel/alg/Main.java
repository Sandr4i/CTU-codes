package cz.cvut.fel.alg;

import java.util.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

class Node {
    protected int value;
    protected int depth;
    protected Node left;
    protected Node right;
    protected Node parent;
    public Node(int value, Node parent){
        this.value = value;
        this.parent = parent;
        this.left = this.right = null;
    }
}

public class Main {
    public static int N;
    public static String[] values;
    public static Node root = null;

    public static void main(String[] args) {
        int ret = mainProcess();
        System.out.println(ret);
    }

    public static int mainProcess() {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        int ret = 0;
        try {
            N = Integer.parseInt(reader.readLine());
            values = reader.readLine().split(" ");
            root = buildTree(0);
            System.out.println(root.left.depth + " " + root.right.depth);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return ret;
    }

    protected static Node buildTree(int id) {
        if (id == N) return null;

        int value = Integer.parseInt(values[id]);

        // Если корень еще не создан, создаем новый корень
        if (root == null) {
            root = new Node(value, null);
            id++; // Переход к следующему значению
        } else {
            // Если корень уже существует, вызываем вспомогательный метод для добавления узла
            addNode(root, value);
        }

        return buildTree(id); // Рекурсивный вызов для следующего значения
    }

    // Вспомогательный метод для добавления узла в дерево
    private static void addNode(Node node, int value) {
        if (value <= node.value) {
            if (node.left == null) {
                node.left = new Node(value, node);
                node.depth++;
                System.out.println("Depth++");
            } else {
                addNode(node.left, value);
            }
        } else if (value > node.value && value <= node.parent.value) {
            if (node.right == null) {
                node.right = new Node(value, node);
                node.depth++;
                System.out.println("Depth++");
            } else {
                addNode(node.right, value);
            }
        }
    }



//    protected static int buildRoad(Node node){
//
//    }
}

