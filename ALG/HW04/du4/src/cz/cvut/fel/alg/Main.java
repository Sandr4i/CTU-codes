package cz.cvut.fel.alg;

import java.util.Scanner;

class GraphNode {
    int distance;
    boolean isVisited;

    public GraphNode() {
        this.distance = -1;
        this.isVisited = false;
    }
}

class Game {
    int rows;
    int cols;
    int colors;
    int[][] field;
    GraphNode[][][] distanceGraph;

    public Game(int rows, int cols, int colors) {
        this.rows = rows;
        this.cols = cols;
        this.colors = colors;
        this.field = new int[rows][cols];
        this.distanceGraph = new GraphNode[colors + 1][rows][cols];

        for (int graphI = 0; graphI <= colors; graphI++) {
            for (int rowI = 0; rowI < rows; rowI++) {
                for (int colI = 0; colI < cols; colI++) {
                    this.distanceGraph[graphI][rowI][colI] = new GraphNode();
                }
            }
        }
    }
}

class Vertex {
    int distance;
    int row;
    int col;
    int graph;
    Vertex next;

    public Vertex(int distance, int row, int col, int graph) {
        this.distance = distance;
        this.row = row;
        this.col = col;
        this.graph = graph;
        this.next = null;
    }
}

class VertexList {
    int size;
    Vertex start;
    Vertex end;

    public VertexList() {
        this.size = 0;
        this.start = null;
        this.end = null;
    }
}

class VertexStorage {
    boolean isNull;
    int distance;
    int row;
    int col;
    int graph;

    public VertexStorage() {
        this.isNull = true;
        this.distance = 0;
        this.row = 0;
        this.col = 0;
        this.graph = 0;
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        int rows, cols, colors;

        rows = scanner.nextInt();
        cols = scanner.nextInt();
        colors = scanner.nextInt();

        Game game = new Game(rows, cols, colors);

        for (int rowI = 0; rowI < rows; rowI++) {
            for (int colI = 0; colI < cols; colI++) {
                game.field[rowI][colI] = scanner.nextInt();
            }
        }

        VertexList list = new VertexList();

        findShortestPath(game, list, rows - 1, 0);

        int minDistance = Integer.MAX_VALUE;
        int graphDistance;

        for (int graphI = 0; graphI <= game.colors; graphI++) {
            graphDistance = game.distanceGraph[graphI][0][game.cols - 1].distance;

            if (graphDistance != -1 && graphDistance < minDistance) {
                minDistance = graphDistance;
            }
        }

        System.out.println(minDistance);

        scanner.close();
    }

    private static void findShortestPath(Game game, VertexList list, int startRow, int startCol) {
        VertexStorage vertexStorage = new VertexStorage();

        addVertex(list, startRow, startCol, 0, 0);

        while (list.size > 0) {
            popVertex(list, vertexStorage);

            if (!game.distanceGraph[vertexStorage.graph][vertexStorage.row][vertexStorage.col].isVisited) {
                game.distanceGraph[vertexStorage.graph][vertexStorage.row][vertexStorage.col].distance = vertexStorage.distance;
                game.distanceGraph[vertexStorage.graph][vertexStorage.row][vertexStorage.col].isVisited = true;

                if (checkCell(game, vertexStorage.graph, vertexStorage.row - 1, vertexStorage.col)) {
                    int newCellGraph = Math.abs(game.field[vertexStorage.row - 1][vertexStorage.col]);

                    if (game.field[vertexStorage.row - 1][vertexStorage.col] < 0 &&
                            newCellGraph != vertexStorage.graph) {
                        addVertex(list, vertexStorage.row - 1, vertexStorage.col, newCellGraph, vertexStorage.distance + 1);
                    } else {
                        addVertex(list, vertexStorage.row - 1, vertexStorage.col, vertexStorage.graph, vertexStorage.distance + 1);
                    }
                }

                if (checkCell(game, vertexStorage.graph, vertexStorage.row, vertexStorage.col + 1)) {
                    int newCellGraph = Math.abs(game.field[vertexStorage.row][vertexStorage.col + 1]);

                    if (game.field[vertexStorage.row][vertexStorage.col + 1] < 0 &&
                            newCellGraph != vertexStorage.graph) {
                        addVertex(list, vertexStorage.row, vertexStorage.col + 1, newCellGraph, vertexStorage.distance + 1);
                    } else {
                        addVertex(list, vertexStorage.row, vertexStorage.col + 1, vertexStorage.graph, vertexStorage.distance + 1);
                    }
                }

                if (checkCell(game, vertexStorage.graph, vertexStorage.row + 1, vertexStorage.col)) {
                    int newCellGraph = Math.abs(game.field[vertexStorage.row + 1][vertexStorage.col]);

                    if (game.field[vertexStorage.row + 1][vertexStorage.col] < 0 &&
                            newCellGraph != vertexStorage.graph) {
                        addVertex(list, vertexStorage.row + 1, vertexStorage.col, newCellGraph, vertexStorage.distance + 1);
                    } else {
                        addVertex(list, vertexStorage.row + 1, vertexStorage.col, vertexStorage.graph, vertexStorage.distance + 1);
                    }
                }

                if (checkCell(game, vertexStorage.graph, vertexStorage.row, vertexStorage.col - 1)) {
                    int newCellGraph = Math.abs(game.field[vertexStorage.row][vertexStorage.col - 1]);

                    if (game.field[vertexStorage.row][vertexStorage.col - 1] < 0 &&
                            newCellGraph != vertexStorage.graph) {
                        addVertex(list, vertexStorage.row, vertexStorage.col - 1, newCellGraph, vertexStorage.distance + 1);
                    } else {
                        addVertex(list, vertexStorage.row, vertexStorage.col - 1, vertexStorage.graph, vertexStorage.distance + 1);
                    }
                }
            }
        }
    }

    private static void addVertex(VertexList list, int row, int col, int graph, int distance) {
        Vertex vertex = new Vertex(distance, row, col, graph);

        if (list.end == null) {
            list.start = vertex;
        } else {
            list.end.next = vertex;
        }

        list.end = vertex;
        list.size++;
    }

    private static void popVertex(VertexList list, VertexStorage vertexStorage) {
        if (list.start == null) {
            vertexStorage.isNull = true;
            return;
        }

        vertexStorage.isNull = false;
        vertexStorage.distance = list.start.distance;
        vertexStorage.row = list.start.row;
        vertexStorage.col = list.start.col;
        vertexStorage.graph = list.start.graph;
        list.start = list.start.next;
        if (list.start == null) {
            list.end = null;
        }
        list.size--;
    }

    private static boolean checkCell(Game game, int graph, int row, int col) {
        if (row < 0 || row >= game.rows || col < 0 || col >= game.cols ||
                game.distanceGraph[graph][row][col].isVisited ||
                (game.field[row][col] > 0 && game.field[row][col] != graph)) {
            return false;
        }
        return true;
    }
}





