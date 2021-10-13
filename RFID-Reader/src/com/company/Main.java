package com.company;


import com.metratec.lib.connection.CommConnectionException;
import com.metratec.lib.rfidreader.RFIDReaderException;
import com.metratec.lib.rfidreader.uhf.PulsarMX;
import com.metratec.lib.rfidreader.uhf.UHFReader;

import java.util.List;


public class Main {

    public static void main(String[] args) {
	// write your code here


        try {
            System.out.println("Start");
            PulsarMX pulsarMX = new PulsarMX("pulsar1","192.168.2.239",10001, UHFReader.READER_MODE.ETS);
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
                    for (int i=0; i < tids.size();i++) {
                        tids.set(i,tids.get(i).substring(0,24));
                        System.out.printf("TID:%s", tids.get(i));

                        double temperatur = readTemperatur(pulsarMX,tids.get(i));
                        System.out.printf(":%f\t", temperatur);

                    }
                    System.out.println();
                    Thread.sleep(1000);

                } catch (RFIDReaderException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }


        } catch (CommConnectionException e)

    {
        e.printStackTrace();
    } catch (RFIDReaderException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }


    }

    public  static  double readTemperatur(PulsarMX pulsarMX,String tid) {

        try {
            pulsarMX.setMask(UHFReader.MEMBANK.TID,tid);
            pulsarMX.setTagData(UHFReader.MEMBANK.USR,"0000",256);
            //Thread.sleep(100);
            List<String> data  = pulsarMX.getTagData(UHFReader.MEMBANK.USR,256,1);
            if (data.size() > 0) {

                int rawdata =  Integer.parseInt(data.get(0),16);
                double temperatur = ((double)rawdata) / 255 * 63.75;
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

}
