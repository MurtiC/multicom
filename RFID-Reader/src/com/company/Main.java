package com.company;


import com.metratec.lib.connection.CommConnectionException;
import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.rfidreader.uhf.PulsarMX;
import com.metratec.lib.rfidreader.uhf.UHFReader;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalTime;

import java.util.List;
import java.util.Scanner;

public class Main {

    public static String CSVSeperator = ";";

    public static void main(String[] args) {
        // write your code here
        try {
            System.out.println("Start");
            PulsarMX pulsarMX = new PulsarMX("pulsar1", "192.168.2.239", 10001, UHFReader.READER_MODE.ETS);
            pulsarMX.connect();
            System.out.println("Connected");

            pulsarMX.setRFInterface(false);
            Thread.sleep(2000);
            pulsarMX.setRFInterface(true);

            while (true) {
                try {
                    pulsarMX.setNoMask();
                    List<String> tids = pulsarMX.getTagTIDs();
                    System.out.printf("Size:%d ", tids.size());
                    for (int i = 0; i < tids.size(); i++) {
                        tids.set(i, tids.get(i).substring(0, 24));
                        System.out.printf("TID:%s", tids.get(i));

                        double temperatur = readTemperatur(pulsarMX, tids.get(i));
                        System.out.printf(":%f\t", temperatur);


                        try {
                            writeTemperature(tids.get(i), temperatur, LocalDate.now(), LocalTime.now());
                            writeCurrentTemperature(tids.get(i), temperatur, LocalDate.now(), LocalTime.now());
                        } catch (IOException e) {
                            e.printStackTrace();
                        }



                    }
                    System.out.println();
                    Thread.sleep(1000);

                } catch (RFIDReaderException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }


        } catch (CommConnectionException e) {
            e.printStackTrace();
        } catch (RFIDReaderException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }


    }

    public static double readTemperatur(PulsarMX pulsarMX, String tid) {

        try {
            pulsarMX.setMask(UHFReader.MEMBANK.TID, tid);
            pulsarMX.setTagData(UHFReader.MEMBANK.USR, "0000", 256);
            //Thread.sleep(100);
            List<String> data = pulsarMX.getTagData(UHFReader.MEMBANK.USR, 256, 1);
            if (data.size() > 0) {

                int rawdata = Integer.parseInt(data.get(0), 16);
                double temperatur = ((double) rawdata) / 255 * 63.75;
                if (rawdata >= 255 || rawdata == 0) {
                    temperatur = -300;
                }
                return temperatur;
            }
        } catch (CommConnectionException e) {
            e.printStackTrace();
        } catch (RFIDReaderException e) {
            e.printStackTrace();
        }
        return -300;
    }

    public static void writeTemperature(String tid, double temperature, LocalDate date, LocalTime time) throws IOException {
        String s = temperature + CSVSeperator + date + " " + time + "\n";
        String filePath = "history/" + tid + ".csv";
        File dir = new File("history");
        File file = new File(filePath);

        if (!dir.exists()) {
            dir.mkdir();
        }

        FileWriter writer = new FileWriter(filePath, true);
        Scanner scanner = new Scanner(file);

        if (!scanner.hasNext()) {
            writer.append("Temperature"+CSVSeperator+"Date Time\n");
            System.out.println(tid + ".csv wurde erstellt");
        }

        writer.append(s);
        writer.close();
        scanner.close();
    }

    public static void writeCurrentTemperature(String tid, double temperature, LocalDate date, LocalTime time) throws IOException {
        String s = temperature + CSVSeperator + date + " " + time + "\n";
        String filePath = "history/" + tid + "current.csv";
        File file = new File(filePath);
        File dir = new File("history");

        if (!dir.exists()) {
            dir.mkdir();
        }

        FileWriter writer = new FileWriter(filePath, false);
        Scanner scanner = new Scanner(file);

        writer.write("Temperature"+CSVSeperator+"Date"+CSVSeperator+"Time\n" + s);
        writer.close();
        scanner.close();
    }

    public static void writeCurrentTemperature2(String tid, double temperature, LocalDate date, LocalTime time){
        String s = tid + CSVSeperator +temperature + CSVSeperator + date + " " +time + "\n";
        String filePath = "history/current.csv";
        String tempPath = "history/current_temp.csv";
        File file = new File(filePath);
        File temp = new File(tempPath);
        File dir = new File("history");

        if(!dir.exists()) dir.mkdir();

        try {
            Scanner scanner = new Scanner(file);
            try {
                //file exists
                FileWriter writer = new FileWriter(temp, true);
                boolean tid_exists = false;
                while(scanner.hasNext()){
                    String row = scanner.next() + "\n";
                    if(row.contains(tid)){
                        writer.append(s);
                        tid_exists = true;
                        break;
                    }else{
                        writer.append(row);
                    }
                }

                if(!tid_exists) writer.append(s);

                scanner.close();
                writer.close();

                if(!file.delete()){
                    System.out.println("File konnte nicht geloscht werden");
                }
                if(!temp.renameTo(file)){
                    System.out.println("File konnte nicht unbenannt werden");
                }
            } catch (IOException e) {
                e.printStackTrace();
            }

        } catch (FileNotFoundException e) {
            //file does not exist
            try {
                //create file and write title + data
                FileWriter writer = new FileWriter(file, true);
                writer.append("TID" + CSVSeperator + "Temperature" + CSVSeperator + "Date Time\n");
                writer.append(s);
                writer.close();
                return;
            } catch (IOException ex) {
                System.out.println("file: current.csv konnte nicht erstellt werden");
            }
        }

    }


}
