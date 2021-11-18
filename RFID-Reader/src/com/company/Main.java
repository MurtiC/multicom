package com.company;

import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.connection.CommConnectionException;

import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.List;


public class Main {

    public static void main(String[] args) {
        System.out.println("Start");
        Reader pulsarMX = new Reader();
        HashMap<String, LocalDateTime> lastContact =  new HashMap<String, LocalDateTime>();
        while (true) {
            try {
                if (pulsarMX == null) {
                    pulsarMX = new Reader();
                    pulsarMX.lastContact = lastContact;
                }
                pulsarMX.connect();
                System.out.println("Connected");
                //switchAntennas
                pulsarMX.updateConfig();
                pulsarMX.setRFInterface(false);
                Thread.sleep(2000);
                pulsarMX.setRFInterface(true);

                while (true) {

                        pulsarMX.setState(pulsarMX.isConnected());
                        pulsarMX.setInputState(1, pulsarMX.getInput(0));
                        pulsarMX.setInputState(2, pulsarMX.getInput(1));

                        List<String> tids = pulsarMX.getTIDs();
                        pulsarMX.addMissingTIDs(tids);

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

                }
            } catch (RFIDReaderException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (CommConnectionException var10) {
                pulsarMX.setState(pulsarMX.isConnected());
                System.out.println("pulsar not connected!");
                var10.printStackTrace();
                lastContact = pulsarMX.lastContact;
                pulsarMX = null;
            }
            catch (Exception ex) {
                ex.printStackTrace();
            }
        }
    }
}
