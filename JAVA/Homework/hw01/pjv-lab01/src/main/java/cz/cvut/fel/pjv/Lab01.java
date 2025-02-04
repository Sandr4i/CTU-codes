package cz.cvut.fel.pjv;

import java.util.Scanner;

public class Lab01 {
   public void homework(String[] args) {
      Scanner sc = new Scanner(System.in);
      String[] op_text1 = {"Zadej scitanec: ", "Zadej mensenec: ", "Zadej cinitel: ", "Zadej delenec: "};
      String[] op_text2 = {"Zadej scitanec: ", "Zadej mensitel: ", "Zadej cinitel: ", "Zadej delitel: "};
      System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
      int operation = sc.nextInt();
      double a;
      double b;
      double result;
      int c;
      String format;
      switch(operation){
         case(1):
            System.out.println(op_text1[operation-1]);
            a = sc.nextDouble();
            System.out.println(op_text2[operation-1]);
            b = sc.nextDouble();
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if ( c < 0) {
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               return;
            }
            format = "%." + c + "f";
            result = a+b;
            System.out.printf(format + " + " + format + " = " + format + "\n", a, b, result);
            break;
         case(2):
            System.out.println(op_text1[operation-1]);
            a = sc.nextDouble();
            System.out.println(op_text2[operation-1]);
            b = sc.nextDouble();
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if ( c < 0) {
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               return;
            }
            format = "%." + c + "f";
            result = a-b;
            System.out.printf(format + " - " + format + " = " + format + "\n", a, b, result);
            break;
         case(3):
            System.out.println(op_text1[operation-1]);
            a = sc.nextDouble();
            System.out.println(op_text2[operation-1]);
            b = sc.nextDouble();
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if ( c < 0) {
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               return;
            }
            format = "%." + c + "f";
            result = a*b;
            System.out.printf(format + " * " + format + " = " + format + "\n", a, b, result);
            break;
         case(4):
            System.out.println(op_text1[operation-1]);
            a = sc.nextDouble();
            System.out.println(op_text2[operation-1]);
            b = sc.nextDouble();
            if ( b == 0){
               System.out.println("Pokus o deleni nulou!");
               return;
            }
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if ( c < 0) {
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               return;
            }
            format = "%." + c + "f";
            result = a/b;
            System.out.printf(format + " / " + format + " = " + format + "\n", a, b, result);
            break;
         default:
            System.out.println("Chybna volba!");
            break;
      }
   }
}