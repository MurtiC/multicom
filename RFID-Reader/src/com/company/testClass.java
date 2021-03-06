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



    public static void main(String[] args) {
        Logger l = new Logger();
        Reader r = new Reader();
        String s = Reader.getCSVCell("files/reader/opcConfig.csv", "ipadress", 1);
        System.out.println(s);

        r.writeCurrentTemperature("E280B0403C0000000C014A97", 20.4, LocalDateTime.now());
        r.lastContact.put("E280B0403C0000000C014A97", LocalDateTime.now());

        r.writeCurrentTemperature("E280B0403C0000000C014A97", -300, LocalDateTime.now().plusDays(1));
        r.writeCurrentTemperature("E280B0403C0000000C014A97", -300, LocalDateTime.now().plusDays(2));
        r.writeCurrentTemperature("E280B0403C0000000C014A97", -300, LocalDateTime.now().plusDays(3));
        r.writeCurrentTemperature("E280B0403C0000000C014A97", -300, LocalDateTime.now().plusDays(4));






    }
}
