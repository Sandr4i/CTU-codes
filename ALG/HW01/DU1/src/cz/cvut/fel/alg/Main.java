package cz.cvut.fel.alg;

import java.io.*;

public class Main {
    public static void main(String[] args) {
        int result = Park();
        System.out.println(result);
    }
    public static int Park() {
        int maximumNumberOfForests = -999;
        int N, K, L, S;
        int[][] forestsPrefix, rocksPrefix;
        int centralParkSize;
        try
        {
            BufferedReader bi = new BufferedReader(new FileReader("/home/sandrai/Desktop/3.semestr/ALG/DU1/datapub/pub01.in"));
            String[] line;
            line = bi.readLine().split(" ");
            N = Integer.parseInt(line[0]);
            K = Integer.parseInt(line[1]);
            L = Integer.parseInt(line[2]);
            S = Integer.parseInt(line[3]);
            forestsPrefix = new int[N+1][N+1];
            rocksPrefix = new int[N+1][N+1];
            centralParkSize = K - 2 * L;
            //Algorithm based on prefix sums
            for (int i = 0; i < N; i++){
                line = bi.readLine().split(" ");
                for (int j = 0; j < N; j++){
                    int value = Integer.parseInt(line[j]);
                    //Calculating prefix sums for forests and rocks in area
                    switch (value){
                        case (1):
                            forestsPrefix[i+1][j+1] = forestsPrefix[i+1][j]
                                    + forestsPrefix[i][j+1]
                                    - forestsPrefix[i][j] + value;
                            rocksPrefix[i+1][j+1] = rocksPrefix[i+1][j]
                                    + rocksPrefix[i][j+1]
                                    - rocksPrefix[i][j];
                            break;
                        case (2):
                            rocksPrefix[i+1][j+1] = rocksPrefix[i+1][j]
                                    + rocksPrefix[i][j+1]
                                    - rocksPrefix[i][j] + value;
                            forestsPrefix[i+1][j+1] = forestsPrefix[i+1][j]
                                    + forestsPrefix[i][j+1]
                                    - forestsPrefix[i][j];
                            break;
                        default:
                            rocksPrefix[i+1][j+1] = rocksPrefix[i+1][j]
                                    + rocksPrefix[i][j+1]
                                    - rocksPrefix[i][j];
                            forestsPrefix[i+1][j+1] = forestsPrefix[i+1][j]
                                    + forestsPrefix[i][j+1]
                                    - forestsPrefix[i][j];
                            break;
                    }
                    //Calculating rocks in possible park area
                    // (if i+1 < K && j+1 < K the park area doesn't match KxK size)
                    if (i+1 >= K && j+1 >= K){
                        int rocksInCenter = rocksPrefix[i+1-L][j+1-L]
                                - rocksPrefix[i+1-L-centralParkSize][j+1-L]
                                - rocksPrefix[i+1-L][j+1-L-centralParkSize]
                                + rocksPrefix[i+1-L-centralParkSize][j+1-L-centralParkSize];
                        //Check for minimum rocks in park center
                        if (rocksInCenter >= 2*S){
                            //Calculating forests in possible park area
                            int forestsInPark = forestsPrefix[i+1][j+1]
                                    - forestsPrefix[i+1-K][j+1]
                                    - forestsPrefix[i+1][j+1-K]
                                    + forestsPrefix[i+1-K][j+1-K];
                            //Check if current number of forests is bigger than maximum
                            if (forestsInPark > maximumNumberOfForests) maximumNumberOfForests = forestsInPark;
                        }
                    }
                }
            }
        }
        catch (IOException e){
            e.printStackTrace();
        }
        return maximumNumberOfForests;
    }
}
