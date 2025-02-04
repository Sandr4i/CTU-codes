//package cz.cvut.fel.alg;
//
//import java.util.ArrayList;
//import java.util.List;
//
//import static java.lang.Math.abs;
//
//class GraphNode {
//    boolean visited;
//    int value;
//    int y,x,z;
//    int visitedCount; // New variable to store the count of visited nodes
//    public GraphNode(int value, int y, int x, int z) {
//        this.visited = false;
//        this.value = value;
//        this.y = y;
//        this.x = x;
//        this.z = z;
//        this.visitedCount = 0;
//    }
//
//    public List<GraphNode> findNeighbors(GraphNode[][][] area, int layer){
//        //GraphNode[] neighbors = new GraphNode[4];
//        List<GraphNode> neighbors = new ArrayList<>();
//        if (this.x > 0 && this.x <= area[0].length - 1) {
//            if (area[this.y][this.x - 1][this.z] != null
//                    && ((area[this.y][this.x - 1][this.z].value <= 0)
//                    || (abs(area[this.y][this.x - 1][this.z].value) == layer))) {
//                neighbors.add(area[this.y][this.x - 1][this.z]);
//            }
//        }
//        if (this.x >= 0 && this.x < area[0].length - 1) {
//            if (area[this.y][this.x + 1][this.z] != null
//                    && ((area[this.y][this.x + 1][this.z].value <= 0)
//                    || (abs(area[this.y][this.x + 1][this.z].value) == layer))) {
//                neighbors.add(area[this.y][this.x + 1][this.z]);
//            }
//        }
//        if (this.y >= 0 && this.y < area.length - 1) {
//            if (area[this.y + 1][this.x][this.z] != null
//                    && ((area[this.y + 1][this.x][this.z].value <= 0)
//                    || (abs(area[this.y + 1][this.x][this.z].value) == layer))) {
//                neighbors.add(area[this.y + 1][this.x][this.z]);
//            }
//        }
//        if (this.y > 0 && this.y <= area.length - 1) {
//            if (area[this.y - 1][this.x][this.z] != null
//                    && ((area[this.y - 1][this.x][this.z].value <= 0)
//                    || (abs(area[this.y - 1][this.x][this.z].value) == layer))) {
//                neighbors.add(area[this.y -1][this.x][this.z]);
//            }
//        }
//        return neighbors;
//    }
//
//    public void incrementVisitedCount() {
//        this.visitedCount++;
//    }
//}


//import java.util.ArrayList;
//import java.util.List;
//import java.util.Scanner;
//
//public class Main {
//    static int M, N, C;
//    static GraphNode gameArea[][][];
//
//    public static void main(String[] args) {
//        Scanner scanner = new Scanner(System.in);
//        M = scanner.nextInt();
//        N = scanner.nextInt();
//        C = scanner.nextInt();
//
//        // Create nodes and add them to the list
//        gameArea = new GraphNode[M][N][C+1];
//        for (int i = M - 1; i >= 0; i--) {
//            for (int j = 0; j < N ; j++) {
//                int value = scanner.nextInt();
//                for (int k = 0; k <= C; k++){
//                    gameArea[i][j][k] = new GraphNode(value, i, j, k);
//                }
//            }
//        }
//        scanner.close();
//
//        // Find the shortest path from starting location to the ending location and print it
//        findShortestPath(gameArea[0][0][0]);
//        System.out.println("------------------");
//        System.out.println(gameArea[3][5][0].visitedCount + " " + gameArea[3][5][1].visitedCount + " " + gameArea[3][5][2].visitedCount);
//    }
//
//    private static void findShortestPath(GraphNode start){
//        List<GraphNode> queue = new ArrayList<>();
//        int layer = 0;
//        GraphNode current;
//        queue.add(start);
//        while (!queue.isEmpty()) {
//            current = queue.get(0);
//            queue.remove(0);
//            if (!current.visited) {
//                current.visitedCount += 1;
//                //if (current.visitedCount < minTime) minTime = current.visitedCount;
//                current.visited = true;
//                if (current.value < 0) {
//                    for (GraphNode neighbor : current.findNeighbors(gameArea, layer)) {
//                        if (!neighbor.visited){
//                            neighbor.visitedCount += current.visitedCount;
//                            queue.add(neighbor);
//                        }
//                    }
//                    layer = -current.value;
//                    int currentTime = current.visitedCount;
//                    current = gameArea[current.y][current.x][layer];
//                    current.visitedCount = currentTime;
//                }
//                for (GraphNode neighbor : current.findNeighbors(gameArea, layer)) {
//                    if (!neighbor.visited){
//                        neighbor.visitedCount += current.visitedCount;
//                        queue.add(neighbor);
//                    }
//                }
//            }
//            System.out.println(current.y + " " + current.x + " " + current.z);
//        }
//        //return minTime;
//    }
//}

