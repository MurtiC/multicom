package com.company;

import com.metratec.lib.connection.CommConnectionException;
import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.rfidreader.uhf.PulsarMX;
import com.metratec.lib.rfidreader.uhf.UHFReader;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.*;
import java.util.stream.Collectors;


public class Reader extends PulsarMX{
    public final static String CSVSeperator = ";";
    public int maxHistoryFiles = 14;
    public HashMap<String, LocalDateTime> lastContact = new HashMap<String, LocalDateTime>();

    /**
     * Creates a PulsarMX Object. The identifier, IP and port is set in the config/pulsarMX.csv file
     */
    public Reader() {
        super(getCSVidentifier(), getCSVip(), getCSVport(), READER_MODE.ETS);
    }

    public void updateConfig(){
        try {
            List<List<String>> csvRead = this.getCSVasArrayList("files/reader/readerConfig.csv");
            List<String> csvConf = csvRead.get(1);

            csvConf.set(0, getCSVidentifier());
            csvConf.set(1, getHardwareRevision());
            csvConf.set(2, getFirmwareRevision());
            csvConf.set(3, getSerialNumber());
            csvConf.set(6, String.valueOf(!isConnected()));

            FileWriter writer = new FileWriter("files/reader/readerConfig.csv", false);

            String s ="";
            for(List<String> line : csvRead){
                s += line.stream().collect(Collectors.joining(CSVSeperator)) + "\n";
            }
            writer.write(s);
            writer.close();

        } catch (RFIDReaderException e) {
            e.printStackTrace();
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
            e.printStackTrace();
            return "";
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
        String pathToOpcConfig = "files/reader/opcConfig.csv";

        return getCSVCell(pathToOpcConfig, 1, 4);
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
        String pathToOpcConfig = "files/reader/opcConfig.csv";
        String port = getCSVCell(pathToOpcConfig, 1, 5);
        if(port.equals("Index out of bounds")) port = "4840";
        return Integer.parseInt(port);
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
        String pathToOpcConfig = "files/reader/opcConfig.csv";

        return getCSVCell(pathToOpcConfig, 1, 0);
    }

    /**
     * This function returns the serialnumber of the PulsarMX Reader
     * @return serialnumber of PulsarMX Reader
     */
    public String getCSVserialnumber(){
        String pathToReaderConfig = "files/reader/readerConfig.csv";

        return getCSVCell(pathToReaderConfig, 1, 3);
    }

    /**
     * This function returns the hardwareversion of the PulsarMX Reader
     * @return hardwareversion of PulsarMX Reader
     */
    public String getCSVhardwareversion(){
        String pathToReaderConfig = "files/reader/readerConfig.csv";

        return getCSVCell(pathToReaderConfig, 1, 1);
    }

    /**
     * This function returns the firmwareversion of the PulsarMX Reader
     * @return firmwareversion of PulsarMX Reader
     */
    public String getCSVfirmwareversion(){
        String pathToReaderConfig = "files/reader/readerConfig.csv";

        return getCSVCell(pathToReaderConfig, 1, 2);
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
     * Writes the measured temperature of the TID into the "files/sensoren/'tid'/'date'.csv file.
     * If the TID was not added yet, the function will NOT create a new directory. It only writes into directories that already exist.
     * There is 1 file for every TID and every Day. The temperature will just be written, if the new Temperature is not equal to the latest temperature.
     * @param tid TID of the TempTag
     * @param temperature measured Temperature
     * @param dateTime date and time of measured Temperature
     */
    public void writeTemperature(String tid, double temperature, LocalDateTime dateTime){
        String s = temperature + CSVSeperator + dateTime.toLocalDate() + " " + dateTime.toLocalTime() + "\n";
        String pathToDir = "files/sensoren/" + tid;
        String pathToFile = pathToDir + "/" + dateTime.toLocalDate() + ".csv";
        File dir = new File(pathToDir);
        File history = new File(pathToFile);

        if(!dir.exists()){
            System.out.println("Der Sensor " + tid + " wurde noch nicht hinzugefügt");
            return;
        }

        try {
            FileWriter writer = new FileWriter(history, true);
            Scanner scanner = new Scanner(history);

            if(!scanner.hasNext()){
                writer.append("Temperature" +CSVSeperator  +"Date Time\n");
                writer.append(s);
                writer.close();
                scanner.close();
                System.out.println(pathToFile +" wurde erstellt");
            }else {
                String r = "";
                while (scanner.hasNextLine()) {
                    r = scanner.nextLine();
                }

                String[] value = r.split(CSVSeperator);
                if (!value[0].equals(String.valueOf(temperature))) {
                    writer.append(s);
                }
                writer.close();
                scanner.close();
            }
            this.deleteOldestFile(pathToDir, dateTime);
        } catch (FileNotFoundException e) {
            System.out.println("File: " + pathToFile + "konnte nicht gelesen werden (writer)");
            e.printStackTrace();
        } catch (IOException e) {
            System.out.println("File: " + pathToFile + "konnte nicht gelesen werden (writer)");
            e.printStackTrace();
        }
    }

    /**
     * Writes the current temperature of the TID sensor into the /current.csv file.
     * If the temperature is -300 (error) and the sensor was not accessible in the specified time (timeout), the sensor will be marked as notConnected;
     * @param tid tid of the temperature sensor
     * @param temperature measured temperature
     */
    public void writeCurrentTemperature(String tid, double temperature, LocalDateTime dateTime){
        try {

        String s;
        if(temperature == -300){
            LocalDateTime lastContactTid = lastContact.get(tid);
            Duration diffBetweenContacts = Duration.between(lastContactTid, dateTime);

            double timeout = Double.valueOf(getCSVCell("files/sensoren/" + tid + "/config.csv", 1, 7)); //antenne soll noch raus dann 6
            if(diffBetweenContacts.toMinutes() > timeout){
                s = tid + CSVSeperator + "" + CSVSeperator + "true\n";
            }else{
                s = tid + CSVSeperator + "" + CSVSeperator + "false\n";
            }
        }else{
            s = tid + CSVSeperator + temperature + CSVSeperator + "false\n";
        }

        String pathToDir = "files/sensoren/" + tid;
        String pathToFile =  pathToDir + "/current.csv";
        File file = new File(pathToFile);
        File dir = new File(pathToDir);

        if(!dir.exists()){
            System.out.println("TagId " + tid + " wurde noch nicht hinzugefügt");
            return;
        }

        try {
            FileWriter writer = new FileWriter(file, false);
            writer.write("TagId;Temperature;NotConnected\n" + s);
            writer.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        }
        catch (NumberFormatException ex) {

        }
    }

    /**
     * Reads the temperature of the sensor with the matching TID.
     * If the reader can not access the sensor, the function will return -300 (not reachable).
     * @param tid TID of the wanted sensor
     * @return the measured temperature or -300 (error)
     */
    public double readTemperature(String tid) {

        try {
            this.setMask(UHFReader.MEMBANK.TID, tid);
            this.setTagData(UHFReader.MEMBANK.USR, "0000", 256);
            //Thread.sleep(100);
            List<String> data = this.getTagData(UHFReader.MEMBANK.USR, 256, 1);
            if (data.size() > 0) { // == 0 wenn tid nicht in reichweite
                int rawdata = Integer.parseInt(data.get(0), 16);
                double temperature = ((double) rawdata) / 255 * 63.75;
                if (rawdata >= 255 || rawdata == 0) {
                    temperature = -300;
                }else{
                    lastContact.put(tid, LocalDateTime.now());
                }
                return temperature;
            }
        } catch (CommConnectionException e) {
            e.printStackTrace();
        } catch (RFIDReaderException e) {
            e.printStackTrace();
        }
        return -300;
    }

    /**
     * If there are more than 'maxHistoryFiles' files for 1 TID, the oldest file will be deleted.
     * The history will only be saved for 'maxHistoryFiles' days.
     * @param pathToDir path to directory
     * @param dateTime dateTime of the measured value
     */
    public void deleteOldestFile(String pathToDir, LocalDateTime dateTime){
        File f1 = new File(pathToDir + "/" + dateTime.toLocalDate().minusDays(maxHistoryFiles) + ".csv");
        if(f1.exists()) {
            f1.delete();
            System.out.println(f1.getName() + " gelöscht");
        }
    }

    /**
     * This function is used to get every added TID from the files Directory.
     * @return TIDs of the added Temperature Sensors
     */
    public List<String> getTIDs(){
        List<String> tids = new ArrayList<>();
        String path = "files/sensoren";
        File[] dir = new File(path).listFiles();
        for(int i = 0; i < dir.length; i++){
            tids.add(dir[i].getName());
        }
        return tids;
    }

    /**
     * Writes the state of the reader into the readerCurrent.csv file.
     * If the reader is connected, it will write: notConnected; false
     * If the reader is not connected, it will write: notConnected; true
     * @param state state of connection of the reader
     */
    public void setState(boolean state){
        String pathToFile = "files/reader/readerCurrent.csv";
        File file = new File(pathToFile);

        List<List<String>> readerCurrentCSV = getCSVasArrayList(pathToFile);
        readerCurrentCSV.get(1).set(2, String.valueOf(!state));

        String s = "";
        for(List l : readerCurrentCSV){
            s += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
        }
        try {
            FileWriter writer = new FileWriter(file, false);
            writer.write(s);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Writes the state of the input Pin into the readerCurrent.csv file.
     * @param pin selected pin
     * @param state state of selected pin
     */
    public void setInputState(int pin, boolean state){
        String pathToFile = "files/reader/readerCurrent.csv";
        File file = new File(pathToFile);

        List<List<String>> readerCurrentCSV = getCSVasArrayList(pathToFile);
        readerCurrentCSV.get(pin+1).set(1, String.valueOf(state));

        String s = "";
        for(List l : readerCurrentCSV){
            s += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
        }
        try {
            FileWriter writer = new FileWriter(file, false);
            writer.write(s);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void addMissingTIDs(List<String> tids){
        String s = "";
        try {
            List<String> allTIDs = this.getTagTIDs();
            for(int i = 0; i < allTIDs.size(); i++){
                if(!tids.contains(allTIDs.get(i).substring(0, 24))){
                    s += allTIDs.get(i) +",";
                }
            }
            List<List<String>> readerCurrentCSV = getCSVasArrayList("files/reader/readerCurrent.csv");
            readerCurrentCSV.get(1).set(3, s);

            String allLines = "";
            for(List l : readerCurrentCSV){
                allLines += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
            }

            FileWriter writer = new FileWriter("files/reader/readerCurrent.csv", false);
            writer.write(allLines);
            writer.close();

        } catch (CommConnectionException e) {
            e.printStackTrace();
        } catch (RFIDReaderException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }



    public static void main(String[] args) {

    }

}