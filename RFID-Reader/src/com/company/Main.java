package com.company;

import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.connection.CommConnectionException;

import java.time.LocalDateTime;
import java.util.List;


public class Main {

    public static void main(String[] args) {
        System.out.println("Start");
        Reader pulsarMX = new Reader();
        while (true) {
            try {
                pulsarMX.connect();
                System.out.println("Connected");
                //switchAntennas
                pulsarMX.updateConfig();
                pulsarMX.setRFInterface(false);
                Thread.sleep(2000);
                pulsarMX.setRFInterface(true);

                while (true) {
                    try {
                        pulsarMX.setState(pulsarMX.isConnected());
                        pulsarMX.setInputState(1, pulsarMX.getInput(1));
                        pulsarMX.setInputState(2, pulsarMX.getInput(2));

                        List<String> tids = pulsarMX.getTIDs();

                        for (int i = 0; i < tids.size(); i++) {
                            if(!pulsarMX.lastContact.containsKey(tids.get(i))){
                                pulsarMX.lastContact.put(tids.get(i), LocalDateTime.now());
                            }
                            System.out.printf("TID:%s", tids.get(i));
                            double temperature = pulsarMX.readTemperature(tids.get(i));
                            System.out.printf(":%f\t", temperature);


                            LocalDateTime dateTime = LocalDateTime.now();
                            pulsarMX.writeTemperature(tids.get(i), temperature, dateTime);
                            pulsarMX.writeCurrentTemperature(tids.get(i), temperature, dateTime);
                        }
                        System.out.println();
                        Thread.sleep(1000L);
                    } catch (InterruptedException var9) {
                        var9.printStackTrace();
                    }
                }
            } catch (RFIDReaderException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (CommConnectionException var10) {
                pulsarMX.setState(pulsarMX.isConnected());
                System.out.println("pulsar not connected!");
                var10.printStackTrace();
            }
        }
    }
}
