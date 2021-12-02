package com.company;

import com.metratec.lib.connection.CommConnectionException;
import com.metratec.lib.rfidreader.RFIDReaderException;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.time.Duration;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class testClass {

    public static String CSVSeperator = ";";

    public static void addMissingTIDs(List<String> tids){
        String s = "";
        try {
            List<String> allTIDs = new ArrayList<>();
            allTIDs.add("E280B0403C0000000C014A97");
            allTIDs.add("E280B0403C0000000C014F84");
            allTIDs.add("E280B0403C0000000C014991");
            allTIDs.add("E280B0403C0000000C014992");

            allTIDs.add("E280B0403C0000000C014000");
            allTIDs.add("E280B0403C0000000C014001");
            allTIDs.add("E280B0403C0000000C014002");
            allTIDs.add("E280B0403C0000000C014003");

            allTIDs.add("E280B0403C0000000C014004");

            for(int i = 0; i < allTIDs.size(); i++){
                if(!tids.contains(allTIDs.get(i).substring(0, 24))){
                    s += allTIDs.get(i) +",";
                }
            }
            s = s.substring(0, s.length()-1); //letztes Komma weg
            List<List<String>> readerCurrentCSV = Reader.getCSVasArrayList("files/reader/readerCurrent.csv");

            if(readerCurrentCSV.get(1).size() < 4){
                List<String> temp = new ArrayList<String>();

                for(int i = 0; i < readerCurrentCSV.get(1).size(); i++){
                    temp.add(readerCurrentCSV.get(1).get(i));
                }
                temp.add(s);
                readerCurrentCSV.set(1, temp);

            }else{
                readerCurrentCSV.get(1).set(3, s);
            }

            String allLines = "";
            for(List l : readerCurrentCSV){
                allLines += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
            }

            FileWriter writer = new FileWriter("files/reader/readerCurrent.csv", false);
            writer.write(allLines);
            writer.close();

        } catch (CommConnectionException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static String getCSVip() {
        String pathToOpcConfig = "files/reader/opcConfig.csv";
        ArrayList<List<String>> csvFile = Reader.getCSVasArrayList(pathToOpcConfig);
        int column = 0;
        if(csvFile.size() > 0){
            for(int i = 0; i < csvFile.get(0).size(); i++){
                if(csvFile.get(0).get(i).equals("IPAdress")){
                    column = i;
                    break;
                }
            }
        }
        String ipAdress = Reader.getCSVCell(pathToOpcConfig, 1 , column);
        if (ipAdress.equals("Index out of bounds")) ipAdress = "1111111";
        return ipAdress;
    }






    public static void main(String[] args) {
        Logger l = new Logger();


        String s = testClass.getCSVip();
        System.out.println(s);


    }
}
