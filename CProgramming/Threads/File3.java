/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Project/Maven2/JavaApp/src/main/java/${packagePath}/${mainClassName}.java to edit this template
 */

package com.mycompany.looping;

/**
 *
 * @author roseg
 */
import java.util.*;
public class Looping {

    public static void main(String[] args) {
        System.out.println("Enter five numbers or -1 to stop");
        Scanner input = new Scanner(System.in);
        for (int i=0; i<5 && i!=-1; i++){
            int num = input.nextInt();
            if (i!=-1) {
                System.out.println("you entered: " + num);
            }
            else {
                System.out.println("Exiting early");
            }
        }
    }
}
