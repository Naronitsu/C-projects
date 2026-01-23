/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package com.mycompany.looping;

/**
 *
 * @author rosea
 */
import java.util.*;
public class GuessinGame {
    public static void main(String[] args) {
        Random r = new Random();
        int guessNum = 1 + r.nextInt(10);  //random number from 0 to 9
        
        int num;
        int tries =0;
        int moreTries = 0;
        final int MAX = 3;
        do {
                
        System.out.println("Guess a number from 1 to 10");
        Scanner input = new Scanner(System.in);
        //**********number range validation ************************
         do {
            num = input.nextInt();
            if ((num<1 || num >10)){
                System.out.println("Number not in range. Try again.");
            }
         } while (num<1 || num >10);
         //*************************************************************
         // From this point forward number is in range
         
         if (num == guessNum) {// IMPORTANT to use == and not = 
                System.out.println("You have guessed the number.");
            } else {
                 System.out.println("You did not guess the number.");
                 tries++;
                 moreTries = MAX - tries;
                 System.out.println("You have " + moreTries + " more tries");
            }
        }while (tries<3 && num!=guessNum);
    }
}
