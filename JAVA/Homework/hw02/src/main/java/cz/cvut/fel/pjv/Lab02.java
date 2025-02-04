package cz.cvut.fel.pjv;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.FileReader;

public class Lab02 {
   public void start(String[] args) {
      homework();
   }

   public void homework() {
      double sum = 0;
      double dev = 0;
      double mean;
      double varience;

      TextIO textIO = new TextIO();

      double[] numbers = new double[10];   // array of numbers
      int i = 0;  // index of array; also used as number counter

      int lineCounter = 0;
      String line = textIO.getLine();  // line from file which we will transform into double later
      lineCounter++;
      while (!line.equals("")) {
         String[] lines = line.split(" ");
         boolean ret = false;

         // transftorming readed line into a number of type int or float or double
         for(String element : lines){
            if(TextIO.isDouble(element)){
               numbers[i] = Double.parseDouble(element);
               i++;
            } else {
               System.err.printf("A number has not been parsed from line %d\n", lineCounter);
               ret = true;
               break;
            }
         }
         if(ret){
            line = textIO.getLine();
            lineCounter++;
            continue;
         }

         if (i == 10) {
            for (double number : numbers) {
               sum += number;
            }
            mean = sum / i;
            for (double number : numbers) {
               dev += Math.pow(number - mean, 2);
            }
            varience = dev / i;
            varience = Math.sqrt(varience);
            System.out.printf("%2d %.3f %.3f\n", i, mean, varience);
            i = 0;
         }
         line = textIO.getLine();
         lineCounter++;
         sum = 0;
         varience = 0;
         mean = 0;
         dev = 0;
      }
      System.err.println("End of input detected!");
      if (i > 1) {
         for (int j = 0; j < i; j++) {
            sum += numbers[j];
         }
         mean = sum / i;
         for (int j = 0; j < i; j++) {
            dev += Math.pow((numbers[j] - mean), 2);
         }
         varience = dev / i;
         varience = Math.sqrt(varience);
         System.out.printf("%2d %.3f %.3f\n", i, mean, varience);
      }
   }
}
