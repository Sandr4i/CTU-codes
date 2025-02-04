import java.util.Scanner;
import java.text.NumberFormat;
public class Lab01 {
   public void homework(String[] args) {
      Scanner sc = new Scanner(System.in);
      System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
      int operation = sc.nextInt();
      float a;
      float b;
      int c;
      NumberFormat df = NumberFormat.getInstance();
      switch(operation){
         case(1):
            System.out.println("Zadej scitanec: ");
            a = sc.nextFloat();
            System.out.println("Zadej scitanec: ");
            b = sc.nextFloat();
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if (c<0){
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               break;
            }
            df.setMaximumFractionDigits(c);
            System.out.println(df.format(a) + " + " + df.format(b) + " = " + df.format((a+b)));
            break;
         case(2):
            System.out.println("Zadej mensenec: ");
            a = sc.nextFloat();
            System.out.println("Zadej mensitel: ");
            b = sc.nextFloat();
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if (c<0){
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               break;
            }
            df.setMaximumFractionDigits(c);
            System.out.println(df.format(a) + " - " + df.format(b) + " = " + df.format((a-b)));
            break;
         case(3):
            System.out.println("Zadej cinitel: ");
            a = sc.nextFloat();
            System.out.println("Zadej cinitel: ");
            b = sc.nextFloat();
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if (c<0){
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               break;
            }
            df.setMaximumFractionDigits(c);
            System.out.println(df.format(a) + " * " + df.format(b) + " = " + df.format((a*b)));
            break;
         case(4):
            System.out.println("Zadej delenec: ");
            a = sc.nextFloat();
            System.out.println("Zadej delitel: ");
            b = sc.nextFloat();
            if (b == 0 ){
               System.out.println("Pokus o deleni nulou!");
               break;
            }
            System.out.println("Zadej pocet desetinnych mist: ");
            c = sc.nextInt();
            if (c<0){
               System.out.println("Chyba - musi byt zadane kladne cislo!");
               break;
            }
            df.setMaximumFractionDigits(c);
            System.out.println(df.format(a) + " / " + df.format(b) + " = " + df.format((a/b)));
            break;
         default:
            System.out.println("Chybna volba!");
      }
   }
}