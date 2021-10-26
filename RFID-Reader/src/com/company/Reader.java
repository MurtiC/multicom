package com.company;

import com.metratec.lib.connection.CommConnectionException;
import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.rfidreader.uhf.PulsarMX;
import com.metratec.lib.rfidreader.uhf.UHFReader;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalTime;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;


public class Reader extends PulsarMX{
    public final static String CSVSeperator = ";";

    /**
     * Creates a PulsarMX Object. The identifier, IP and port is set in the config/pulsarMX.csv file
     */
    public Reader() {
        super(getCSVidentifier(), getCSVip(), getCSVport(), READER_MODE.ETS);
    }

    public void updateConfig(){
        try {
            /*String serialnumber = this.getSerialNumber();
            String hardwareversion = this.getHardwareRevision();
            String firwareversion = this.getFirmwareRevision();
             */
            String serialnumber = "111123";
            String hardwareversion = "v.923";
            String firwareversion = "asd.2131";

            List<List<String>> csvRead = this.getCSVasArrayList("config/pulsarMX.csv");
            List<String> csvConf = csvRead.get(1);

            csvConf.set(3, serialnumber);
            csvConf.set(4, hardwareversion);
            csvConf.set(5, firwareversion);

            FileWriter writer = new FileWriter("config/pulsarMX.csv", false);
            writer.write(csvRead.get(0).stream().collect(Collectors.joining(CSVSeperator)) + "\n");
            writer.write(csvConf.stream().collect(Collectors.joining(CSVSeperator)));
            writer.close();

        /*} catch (RFIDReaderException e) {

            e.printStackTrace();*/
        } catch (CommConnectionException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    /**
     * Returns the String value of the Cell (row, column) in the file (filepath).
     * The first Cell of a csv-file is (0/0).
     * If the parameter row or column is larger than the actual size of the file, the function returns "Index out of bounds"
     * @param filePath the path to the selected file
     * @param row   row number of the selected cell (starts with 0)
     * @param column column number of the selected cell (starts with 0)
     * @return the value of the cell(row,colum) in the file (filepath)
     */
    public static String getCSVCell(String filePath, int row, int column){
        try {
            Scanner scanner = new Scanner(new File(filePath));
            List<List<String>> lines = getCSVasArrayList(filePath);

            if(row > (lines.size() -1) || column > (lines.get(row).size() -1)){
                return "Index out of bounds";
            }

            return lines.get(row).get(column);
        } catch (FileNotFoundException e) {
            return String.valueOf(e);
        }

    }

    /**
     * Returns the selected csv File as a List<List<String>>.
     * Each line of the csv File is saved in a List<String>.
     * @param filePath path to the selected csv-file
     * @return the data of the selected csv-file
     */
    public static List<List<String>> getCSVasArrayList(String filePath){
        try {
            Scanner scanner = new Scanner(new File(filePath));
            List<List<String>> lines = new ArrayList<>();

            while(scanner.hasNext()){
                String line = scanner.nextLine();
                String[] values = line.split(CSVSeperator);
                lines.add(Arrays.asList(values));
            }
            scanner.close();

            return lines;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }

    }

    /**
     * This function will only be used for the super-constructor.
     * It only returns the value of the "ip-address cell" in the config/pulsarMX.csv-file.
     * This should, but don't have to be, the current ip-address of the RFID-Reader.
     * If the value of the cell has been changed after the Reader has been started, but the Reader has not been
     * restarted yet, the function will NOT return the current IP-address of the reader.
     * @return the value of the "ip-address cell" in the config/pulsarMX.csv-file
     */
    private static String getCSVip(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);
        if(!configDir.exists() || !configFile.exists()){
            createConfigMX();
        }
        return getCSVCell("config/pulsarMX.csv", 1, 0);
    }

    /**
     * This function will only be used for the super-constructor.
     * It only returns the value of the "port cell" in the config/pulsarMX.csv-file.
     * This should, but don't have to be, the current port of the RFID-Reader.
     * If the value of the cell has been changed after the Reader has been started, but the Reader has not been
     * restarted yet, the function will NOT return the current port of the reader.
     * @return the value of the "port cell" in the config/pulsarMX.csv-file
     */
    private static int getCSVport(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);
        if(!configDir.exists() || !configFile.exists()){
            createConfigMX();
        }
        return Integer.parseInt(getCSVCell("config/pulsarMX.csv", 1, 1));
    }

    /**
     * This function will only be used for the super-constructor.
     * It only returns the value of the "identifier cell" in the config/pulsarMX.csv-file.
     * This should, but don't have to be, the current identifier of the RFID-Reader.
     * If the value of the cell has been changed after the Reader has been started, but the Reader has not been
     * restarted yet, the function will NOT return the current identifier of the reader.
     * @return the value of the "identifier cell" in the config/pulsarMX.csv-file
     */
    private static String getCSVidentifier(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);
        if(!configDir.exists() || !configFile.exists()){
            createConfigMX();
        }
        return getCSVCell("config/pulsarMX.csv", 1, 2);
    }

    /**
     * This function returns the serialnumber of the PulsarMX Reader
     * @return serialnumber of PulsarMX Reader
     */
    public String getCSVserialnumber(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);
        if(!configDir.exists() || !configFile.exists()){
            createConfigMX();
        }
        return getCSVCell("config/pulsarMX.csv", 1, 3);
    }

    /**
     * This function returns the hardwareversion of the PulsarMX Reader
     * @return hardwareversion of PulsarMX Reader
     */
    public String getCSVhardwareversion(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);
        if(!configDir.exists() || !configFile.exists()){
            createConfigMX();
        }
        return getCSVCell("config/pulsarMX.csv", 1, 4);
    }

    /**
     * This function returns the firmwareversion of the PulsarMX Reader
     * @return firmwareversion of PulsarMX Reader
     */
    public String getCSVfirmwareversion(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);
        if(!configDir.exists() || !configFile.exists()){
            createConfigMX();
        }
        return getCSVCell("config/pulsarMX.csv", 1, 5);
    }

    /**
     * @return value of function getCSVip(); IP-Address
     */
    public String getIP(){
        return getCSVip();
    }

    /**
     * @return value of function getCSVport(); Port
     */
    public int getPort(){
        return getCSVport();
    }

    /**
     * This function creates the config file for the PulsrMX Reader.
     * The standard values are:
     * IP-Address:  192.168.2.239
     * Port:        10001
     * Identifier:  pulsarMX
     * Rest is set to null, because it is not known yet.
     */
    public static void createConfigMX(){
        File configDir = new File("config");
        String filePath = "config/pulsarMX.csv";
        File configFile = new File(filePath);

        String title = "ip" + CSVSeperator + "port" + CSVSeperator + "identifier" + CSVSeperator + "serialnumber" + CSVSeperator + "hardwareversion" + CSVSeperator + "firmwareversion\n";
        String standard = "192.168.2.239" + CSVSeperator + "10001" + CSVSeperator + "pulsarMX" + CSVSeperator + null + CSVSeperator + null + CSVSeperator + null;

        if(!configDir.exists()){            //config existiert noch nicht
            configDir.mkdir();
            try {
                FileWriter writer = new FileWriter(filePath, false);
                writer.write(title);
                writer.write(standard);
                writer.close();
                return;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        try {
            Scanner scanner = new Scanner(configFile);

            if(!scanner.hasNext()){        //config existiert aber ist leer
                try {
                    FileWriter writer = new FileWriter(filePath, false);
                    writer.write(title);
                    writer.write(standard);
                    writer.write("exisitert aber leer");
                    writer.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }else if(scanner.hasNext()){        //config exisitert und hat bereits werte
            /*
            >entweder leeren und standardwerte einstellen
            >oder werte drin lassen und nichts ändern
            >oder nur bestimmte werte ändern
	            >welche werte lassen und welche ändern?
             */

                System.out.println("Existiert und hat Werte");

            }
            scanner.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

    }

    /**
     * Version 1 of writeTemperature
     * Writes the new Temperature into the history file.
     * If the history file for the TID does not exist yet, it creates it.
     * @param tid TID of the TempTag
     * @param temperature measured temperature,
     * @param date date of measured temperature
     * @param time time of measured temperature
     * @throws IOException FileWriter has no access to the file
     */
    public void writeTemperature(String tid, double temperature, LocalDate date, LocalTime time) throws IOException{
        String s = temperature + CSVSeperator + date + CSVSeperator +time + "\n";
        String filePath = "history/" +tid + ".csv";
        File file = new File(filePath);
        File dir = new File("history");

        if(!dir.exists()){
            dir.mkdir();
        }

        FileWriter writer = new FileWriter(filePath, true);
        Scanner scanner = new Scanner(file);

        if(!scanner.hasNext()){
            writer.append("Temperature" +CSVSeperator  +"Date" +CSVSeperator +"Time\n");
            System.out.println(tid +".csv wurde erstellt");
        }

        writer.append(s);
        writer.close();
        scanner.close();
    }

    /**
     * Version 2 of writeCurrentTemperature
     * Saves only the current Temperature of every TID in history/current.csv.
     * If there is already a measured Temperature for the selected TID, it will be overwritten.
     * @param tid TID of the TempTag
     * @param temperature measured Temperature
     * @param date date of measured Temperature
     * @param time time of measured Temperature
     */
    public void writeCurrentTemperature2(String tid, double temperature, LocalDate date, LocalTime time){
        String s = tid + CSVSeperator +temperature + CSVSeperator + date + CSVSeperator +time + "\n";
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
                writer.append("TID" + CSVSeperator + "Temperature" + CSVSeperator + "Date" + CSVSeperator + "Time\n");
                writer.append(s);
                writer.close();
                return;
            } catch (IOException ex) {
                System.out.println("file: current.csv konnte nicht erstellt werden");
            }
        }

    }

    /**
     *
     * @param tid
     * @return
     */
    public double readTemperature(String tid) {

        try {
            this.setMask(UHFReader.MEMBANK.TID, tid);
            this.setTagData(UHFReader.MEMBANK.USR, "0000", 256);
            //Thread.sleep(100);
            List<String> data = this.getTagData(UHFReader.MEMBANK.USR, 256, 1);
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

    /**
     * Version 2 of Write Temperature
     * Writes the measured temperature in the history file. There is 1 file for every TID and every Day.
     * The temperature will just be written, if the new Temperature is not equal to the latest temperature.
     * @param tid TID of the TempTag
     * @param temperature measured Temperature
     * @param date date of measured Temperature
     * @param time time of measured Temperature
     * @throws IOException FileWriter has no access to the file
     */
    public void writeTemperature2(String tid, double temperature, LocalDate date, LocalTime time) throws IOException{
        String s = temperature + CSVSeperator + date + CSVSeperator +time + "\n";
        String filePath = "history/" +tid + date + ".csv";
        File file = new File(filePath);
        File dir = new File("history");

        if(!dir.exists()){
            dir.mkdir();
        }

        FileWriter writer = new FileWriter(filePath, true);
        Scanner scanner = new Scanner(file);

        if(!scanner.hasNext()){
            writer.append("Temperature" +CSVSeperator  +"Date" +CSVSeperator +"Time\n");
            writer.append(s);
            writer.close();
            scanner.close();
            System.out.println(filePath +" wurde erstellt");
            this.deleteOldestFile(tid, date);
        }else{
            //mit letzten wert vergleichen
            String r = "";
            while(scanner.hasNextLine()){
                r = scanner.nextLine();
            }

            String[] value = r.split(CSVSeperator);
            if(value[0].equals(String.valueOf(temperature))){
                System.out.println("temperatur daten gleich");
            }else{
                System.out.println("Temperatur angehängt");
                writer.append(s);
            }

            writer.close();
            scanner.close();
        }

    }

    /**
     * If there are more than 14 files for 1 TID, the oldest file will be deleted.
     * The history will only be saved for 14 days
     * @param tid TID of the TempTag
     * @param date date of the measured value
     */
    public void deleteOldestFile(String tid, LocalDate date){
        File f1 = new File("history/" +tid + date.minusDays(14) + ".csv");
        if(f1.exists()) {
            f1.delete();
            System.out.println(f1.getName() + " gelöscht");
        }
    }




    public static void main(String[] args) {

    }

}