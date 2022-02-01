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


public class Reader extends PulsarMX {
    public final static String CSVSeperator = ";";
    public int maxHistoryFiles = 14;
    public HashMap<String, LocalDateTime> lastContact = new HashMap<String, LocalDateTime>();
    private Logger logger = new Logger();

    /**
     * Creates a PulsarMX Object. The identifier, IP and port is set in the files/reader/opcConfig.csv file
     */
    public Reader() {
        super(getCSVidentifier(), getCSVip(), getCSVport(), READER_MODE.ETS);
    }

    /**
     * Updates the files/reader/readerConfig.csv file.
     * Writes the Hardware-, Firmware Revision, Serialnumber and State of connection
     */
    public void updateConfig() {
        try {
            String filePath = "files/reader/readerConfig.csv";
            File f1 = new File(filePath);
            if (!f1.exists()) {
                FileWriter writer = new FileWriter(filePath, true);
                writer.write("Id;Hardwarerevision;Softwarerevision;Serialnumber;\n");
                writer.write(getCSVidentifier() + CSVSeperator);
                writer.write(getHardwareRevision() + CSVSeperator);
                writer.write(getFirmwareRevision() + CSVSeperator);
                writer.write(getSerialNumber() + CSVSeperator);
                writer.close();
            }else{
                List<List<String>> csvRead = this.getCSVasArrayList(filePath);
                List<String> csvConf = csvRead.get(1);

                csvConf.set(0, getCSVidentifier());
                csvConf.set(1, getHardwareRevision());
                csvConf.set(2, getFirmwareRevision());
                csvConf.set(3, getSerialNumber());

                FileWriter writer = new FileWriter(filePath, false);

                String s = "";
                for (List<String> line : csvRead) {
                    s += line.stream().collect(Collectors.joining(CSVSeperator)) + "\n";
                }
                writer.write(s);
                writer.close();
            }

        } catch (RFIDReaderException e) {
            logger.log("Reader.updateConfig(): RFIDReaderException: " + e.toString());
        } catch (CommConnectionException e) {
            logger.log("Reader.updateConfig(): CommConnectionException: " + e.toString());
        } catch (IOException e) {
            logger.log("Reader.updateConfig(): IOException: " + e.toString());
        }
    }


    /**
     * Returns the String value of the Cell (row, column) in the file (filepath).
     * The first Cell of a csv-file is (0/0).
     * If the parameter row or column is larger than the actual size of the file, the function returns "Index out of bounds"
     *
     * @param filePath the path to the selected file
     * @param row      row number of the selected cell (starts with 0)
     * @param column   column number of the selected cell (starts with 0)
     * @return the value of the cell(row,colum) in the file (filepath)
     */
    public static String getCSVCellOld(String filePath, int row, int column) {
        ArrayList<List<String>> lines = getCSVasArrayList(filePath);

        if (row > (lines.size() - 1) || column > (lines.get(row).size() - 1)) {
            return "Index out of bounds";
        }

        return lines.get(row).get(column);

    }

    public static String getCSVCell(String filePath, String attribute, int row) {
        ArrayList<List<String>> lines = getCSVasArrayList(filePath);
        int column = -1;
        boolean found = false;

        if (lines.size() > 0) {
            for (int i = 0; i < lines.get(0).size(); i++) {
                if (lines.get(0).get(i).toLowerCase(Locale.ROOT).equals(attribute.toLowerCase(Locale.ROOT))) {
                    column = i;
                    found = true;
                    break;
                }
            }
        }

        if (found == false) {
            return "Index out of bounds";
        }
        if (column == -1) {
            if (row > (lines.size() - 1) || column > (lines.get(row).size() - 1)) {
                return "Index out of bounds";
            }

        }

        return lines.get(row).get(column);

    }

    /**
     * Returns the selected csv File as a ArrayList<List<String>>.
     * Each line of the csv File is saved in a List<String>.
     *
     * @param filePath path to the selected csv-file
     * @return the data of the selected csv-file
     */
    public static ArrayList<List<String>> getCSVasArrayList(String filePath) {
        try {
            Scanner scanner = new Scanner(new File(filePath));
            ArrayList<List<String>> lines = new ArrayList<>();

            while (scanner.hasNext()) {
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
     *
     * @return the value of the "ip-address cell" in the config/pulsarMX.csv-file
     */
    private static String getCSVip() {
        String pathToOpcConfig = "files/reader/opcConfig.csv";
        String ipAdress = getCSVCell(pathToOpcConfig, "IPAdress", 1);
        if (ipAdress.equals("Index out of bounds")) ipAdress = "192.168.2.239";
        return ipAdress;
    }

    /**
     * This function will only be used for the super-constructor.
     * It only returns the value of the "port cell" in the config/pulsarMX.csv-file.
     * This should, but don't have to be, the current port of the RFID-Reader.
     * If the value of the cell has been changed after the Reader has been started, but the Reader has not been
     * restarted yet, the function will NOT return the current port of the reader.
     *
     * @return the value of the "port cell" in the config/pulsarMX.csv-file
     */
    private static int getCSVport() {
        String pathToOpcConfig = "files/reader/opcConfig.csv";
        String port = getCSVCell(pathToOpcConfig, "Port", 1);
        if (port.equals("Index out of bounds")) port = "10001";
        return Integer.parseInt(port);
    }

    /**
     * This function will only be used for the super-constructor.
     * It only returns the value of the "identifier cell" in the config/pulsarMX.csv-file.
     * This should, but don't have to be, the current identifier of the RFID-Reader.
     * If the value of the cell has been changed after the Reader has been started, but the Reader has not been
     * restarted yet, the function will NOT return the current identifier of the reader.
     *
     * @return the value of the "identifier cell" in the config/pulsarMX.csv-file
     */
    private static String getCSVidentifier() {
        String pathToOpcConfig = "files/reader/opcConfig.csv";
        String identifier = getCSVCell(pathToOpcConfig, "Id", 1);
        if (identifier.equals("Index out of bounds")) identifier = "reader1";
        return identifier;
    }

    /**
     * This Function creates the opcConfig file with standard values.
     * The standard values are:
     * IP-Address:  192.168.2.239
     * Port:        10001
     * Identifier:  reader1
     */
    public static void createOpcConfig() {
        File opcConfigFile = new File("files/reader/opcConfig.csv");

        String title = "Id" + CSVSeperator + "Name" + CSVSeperator + "Description" + CSVSeperator + "Location" + CSVSeperator + "IPAdress" + CSVSeperator + "Port" + CSVSeperator + "Pin" + CSVSeperator + "Mode" + CSVSeperator + "State" + CSVSeperator + "\n";
        String standard = "reader1" + CSVSeperator + "pulsarMX" + CSVSeperator + "Lesegeraet" + CSVSeperator + "" + CSVSeperator + "192.168.2.239" + CSVSeperator + "10001" + CSVSeperator + CSVSeperator + CSVSeperator + CSVSeperator + "\n";

        try {
            FileWriter writer = new FileWriter(opcConfigFile.getPath(), false);
            writer.write(title + standard);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    /**
     * Writes the measured temperature of the TID into the "files/sensoren/'tid'/'date'.csv file.
     * If the TID was not added yet, the function will NOT create a new directory. It only writes into directories that already exist.
     * There is 1 file for every TID and every Day. The temperature will just be written, if the new Temperature is not equal to the latest temperature.
     *
     * @param tid         TID of the TempTag
     * @param temperature measured Temperature
     * @param dateTime    date and time of measured Temperature
     */
    public void writeTemperature(String tid, double temperature, LocalDateTime dateTime) {
        String s = temperature + CSVSeperator + dateTime.toLocalDate() + " " + dateTime.toLocalTime() + "\n";
        String pathToDir = "files/sensoren/" + tid;
        String pathToFile = pathToDir + "/" + dateTime.toLocalDate() + ".csv";
        File dir = new File(pathToDir);
        File history = new File(pathToFile);

        if (!dir.exists()) {
            System.out.println("Der Sensor " + tid + " wurde noch nicht hinzugefügt");
            return;
        }

        try {
            FileWriter writer = new FileWriter(history, true);
            Scanner scanner = new Scanner(history);

            if (!scanner.hasNext()) {
                writer.append("Temperatur" + CSVSeperator + "Date Time\n");
                writer.append(s);
                writer.close();
                scanner.close();
                System.out.println(pathToFile + " wurde erstellt");
            } else {
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
            logger.log("Reader.writeTemperature(): FileNotFoundException: " + e.toString());
        } catch (IOException e) {
            logger.log("Reader.writeTemperature(): IOException: " + e.toString());
        }
    }

    /**
     * Writes the current temperature of the TID sensor into the /current.csv file.
     * If the temperature is -300 (error) and the sensor was not accessible in the specified time (timeout), the sensor will be marked as notConnected;
     *
     * @param tid         tid of the temperature sensor
     * @param temperature measured temperature
     */
    public void writeCurrentTemperature(String tid, double temperature, LocalDateTime dateTime) {
        try {
            String s;
            if (temperature == -300) {
                LocalDateTime lastContactTid = lastContact.get(tid);
                Duration diffBetweenContacts = Duration.between(lastContactTid, dateTime);

                double timeout = Double.valueOf(getCSVCell("files/sensoren/" + tid + "/config.csv", "timeout", 1)); //antenne soll noch raus dann 6
                if (diffBetweenContacts.toMinutes() > timeout) {
                    if (getCSVCell("files/sensoren/" + tid + "/current.csv", "NotConnected", 1).toLowerCase(Locale.ROOT).equals("false")) {
                        logger.log(tid + " Timeout reached!");
                    }
                    s = tid + CSVSeperator + "" + CSVSeperator + "true\n";
                } else {
                    String oldTemperature = getCSVCell("files/sensoren/" + tid + "/current.csv", "Temperatur", 1);
                    s = tid + CSVSeperator + oldTemperature + CSVSeperator + "false\n";
                }
            } else {
                s = tid + CSVSeperator + temperature + CSVSeperator + "false\n";
            }

            String pathToDir = "files/sensoren/" + tid;
            String pathToFile = pathToDir + "/current.csv";
            File file = new File(pathToFile);
            File dir = new File(pathToDir);

            if (!dir.exists()) {
                System.out.println("TagId " + tid + " wurde noch nicht hinzugefügt");
                return;
            }

            try {
                FileWriter writer = new FileWriter(file, false);
                writer.write("TagId;Temperatur;NotConnected\n" + s);
                writer.close();
            } catch (IOException e) {
                logger.log("Reader.writeCurrentTemperature(): IOException: " + e.toString());
            }

        } catch (NumberFormatException e) {
            logger.log("Reader.writeCurrentTemperature(): NumberFormatException: " + e.toString());
        }
    }

    /**
     * Reads the temperature of the sensor with the matching TID.
     * If the reader can not access the sensor, the function will return -300 (not reachable).
     *
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
                } else {
                    lastContact.put(tid, LocalDateTime.now());
                }
                return temperature;
            }
        } catch (CommConnectionException e) {
            logger.log("Reader.readTemperature(): CommConnectionException: " + e.toString());
        } catch (RFIDReaderException e) {
            logger.log("Reader.readTemperature(): RFIDReaderException: " + e.toString());
        }
        return -300;
    }

    /**
     * If there are more than 'maxHistoryFiles' files for 1 TID, the oldest file will be deleted.
     * The history will only be saved for 'maxHistoryFiles' days.
     *
     * @param pathToDir path to directory
     * @param dateTime  dateTime of the measured value
     */
    public void deleteOldestFile(String pathToDir, LocalDateTime dateTime) {
        File f1 = new File(pathToDir + "/" + dateTime.toLocalDate().minusDays(maxHistoryFiles) + ".csv");
        if (f1.exists()) {
            f1.delete();
            System.out.println(f1.getName() + " gelöscht");
        }
    }

    /**
     * This function is used to get every added TID from the files Directory.
     *
     * @return TIDs of the added Temperature Sensors
     */
    public List<String> getTIDs() {
        List<String> tids = new ArrayList<>();
        String path = "files/sensoren";
        File[] dir = new File(path).listFiles();
        for (int i = 0; i < dir.length; i++) {
            tids.add(dir[i].getName());
        }
        return tids;
    }

    /**
     * Writes the state of the reader into the readerCurrent.csv file.
     * If the reader is connected, it will write: notConnected; false
     * If the reader is not connected, it will write: notConnected; true
     *
     * @param state state of connection of the reader
     */
    public void setReaderConnectionState(boolean state) {
        String pathToFile = "files/reader/readerCurrent.csv";
        File file = new File(pathToFile);

        ArrayList<List<String>> readerCurrentCSV = getCSVasArrayList(pathToFile);
        readerCurrentCSV.get(1).set(2, String.valueOf(!state));

        String s = "";
        for (List l : readerCurrentCSV) {
            s += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
        }
        try {
            FileWriter writer = new FileWriter(file, false);
            writer.write(s);
            writer.close();
        } catch (IOException e) {
            logger.log("Reader.setState(): IOException: " + e.toString());
        }
    }

    /**
     * Writes the state of the input Pin into the readerCurrent.csv file.
     *
     * @param pin   selected pin
     * @param state state of selected pin
     */
    public void setInputPinState(int pin, boolean state) {
        String pathToFile = "files/reader/readerCurrent.csv";
        File file = new File(pathToFile);

        ArrayList<List<String>> readerCurrentCSV = getCSVasArrayList(pathToFile);
        readerCurrentCSV.get(pin + 1).set(1, String.valueOf(state));

        String s = "";
        for (List l : readerCurrentCSV) {
            s += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
        }
        try {
            FileWriter writer = new FileWriter(file, false);
            writer.write(s);
            writer.close();
        } catch (IOException e) {
            logger.log("Reader.setInputState(): IOException: " + e.toString());
        }
    }

    /**
     * Compares the added Temperature Sensors with the accessible Sensors and writes the TIDs of the missing Sensor to the readerCurrent.csv file.
     *
     * @param tids List of added Temperature Sensors
     */
    public void addMissingTIDs(List<String> tids) {
        String s = "";
        try {
            this.setNoMask();
            List<String> allTIDs = this.getTagTIDs();
            for (int i = 0; i < allTIDs.size(); i++) {
                if (!tids.contains(allTIDs.get(i).substring(0, 24))) {
                    s += allTIDs.get(i).substring(0, 24) + ",";
                }
            }
            if (s.length() > 0) s = s.substring(0, s.length() - 1); //letztes Komma weg
            ArrayList<List<String>> readerCurrentCSV = this.getCSVasArrayList("files/reader/readerCurrent.csv");

            if (readerCurrentCSV.get(1).size() < 4) {
                List<String> temp = new ArrayList<>();

                for (int i = 0; i < readerCurrentCSV.get(1).size(); i++) {
                    temp.add(readerCurrentCSV.get(1).get(i));
                }
                temp.add(s);
                readerCurrentCSV.set(1, temp);

            } else {
                readerCurrentCSV.get(1).set(3, s);
            }

            String allLines = "";
            for (List l : readerCurrentCSV) {
                allLines += l.stream().collect(Collectors.joining(CSVSeperator)) + ";\n";
            }

            FileWriter writer = new FileWriter("files/reader/readerCurrent.csv", false);
            writer.write(allLines);
            writer.close();

        } catch (CommConnectionException e) {
            logger.log("Reader.addMissingTIDs(): CommConnectionException: " + e.toString());
        } catch (RFIDReaderException e) {
            logger.log("Reader.addMissingTIDs(): RFIDReaderException: " + e.toString());
        } catch (IOException e) {
            logger.log("Reader.addMissingTIDs(): IOException: " + e.toString());
        }
    }

}