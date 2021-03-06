package com.company;

import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.connection.CommConnectionException;

import java.io.File;
import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.List;


public class Main {

    public static void main(String[] args) {
        Logger logger = new Logger();
        logger.log("Main: Reader Start!");
        System.out.println("Main: Reader Start!");

        File opcConfigFile = new File("files/reader/opcConfig.csv");
        if (!opcConfigFile.exists()) {
            logger.log("Main: " + opcConfigFile.getPath() + " does not exists!");
            Reader.createOpcConfig();
            logger.log("Main: files/reader/opcConfig.csv file created with standard values (Identifier: reader1, IP: 192.168.2.239, Port: 10001");
        }

        Reader pulsarMX = new Reader();
        HashMap<String, LocalDateTime> lastContacts = new HashMap<String, LocalDateTime>();
        while (true) {
            try {
                if (pulsarMX == null) {
                    pulsarMX = new Reader();
                    pulsarMX.lastContact = lastContacts;
                }
                pulsarMX.connect();
                logger.log("Main: Reader Connected!");
                System.out.println("Main: Reader Connected!");
                pulsarMX.setNoMask();
                //switchAntennas
                pulsarMX.updateConfig();
                pulsarMX.setRFInterface(false);
                Thread.sleep(2000);
                pulsarMX.setRFInterface(true);

                while (true) {
                    pulsarMX.setReaderConnectionState(pulsarMX.isConnected());

                    List<String> tids = pulsarMX.getTIDs();
                    pulsarMX.addMissingTIDs(tids);

                    for (int i = 0; i < tids.size(); i++) {
                        if (!pulsarMX.lastContact.containsKey(tids.get(i))) {
                            pulsarMX.lastContact.put(tids.get(i), LocalDateTime.now());
                        }
                        double temperature = pulsarMX.readTemperature(tids.get(i));

                        LocalDateTime dateTime = LocalDateTime.now();
                        pulsarMX.writeCurrentTemperature(tids.get(i), temperature, dateTime);
                        pulsarMX.writeTemperature(tids.get(i), temperature, dateTime);
                        pulsarMX.checkMaxTemp(tids.get(i), temperature);
                        pulsarMX.checkMinTemp(tids.get(i), temperature);
                    }

                }
            } catch (RFIDReaderException e) {
                logger.log("Main: RFIDReaderException: " + e.toString());
            } catch (InterruptedException e) {
                logger.log("Main: InterruptedException: " + e.toString());
            } catch (CommConnectionException e) {
                pulsarMX.setReaderConnectionState(pulsarMX.isConnected());
                logger.log("Main: Reader is not connected!");
                logger.log("Main: CommConnectionException: " + e.toString());
                lastContacts = pulsarMX.lastContact;
                pulsarMX = null;
            } catch (Exception e) {
                System.out.println(e.getCause());
                logger.log("Main: Exception: " + e.toString());
            }
        }
    }
}
