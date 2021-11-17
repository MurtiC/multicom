package com.company;

import java.io.FileWriter;
import java.io.IOException;
import java.time.Duration;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.List;
import java.util.stream.Collectors;

public class testClass {

    public static void main(String[] args) {
        /*LocalTime time = LocalTime.of(0,14,59);
        LocalDate date = LocalDate.of(2021,11,15);

        LocalTime oldTime = LocalTime.of(23,45,0);
        LocalDate oldDate = LocalDate.of(2021,11,14);

        double diff = Duration.between(oldTime, time).toMinutes();

        if(date.compareTo(oldDate) > 0) {
            diff += 1440;
        }

        System.out.println("Differenz: " +diff);

        System.out.println(time);
        System.out.println(date);
        System.out.println(oldTime);
        System.out.println(oldDate);


        LocalDateTime dateTime = LocalDateTime.of(2021,11,15,0,14,59);
        LocalDateTime oldDateTime = LocalDateTime.of(1,1,1,0,0,0);

        Duration asd = Duration.between(oldDateTime, dateTime);
        System.out.println("LocalDateTime: " +asd.toMinutes());
        System.out.println(dateTime);


        Reader r = new Reader();

        List<String> tags = r.getTIDs();

        if(!r.lastContact.containsKey(tags.get(0))){
            r.lastContact.put(tags.get(0), LocalDateTime.now());
        }
        System.out.println(tags.get(0));

        Reader r = new Reader();

        for(int i = 0; i < 20; i++){
            r.setState(true);
            r.setInputState(1,true);
            r.setInputState(2,false);

            List<String> tags = r.getTIDs();

            for(String tag : tags){
                if(!r.lastContact.containsKey(tag)){
                    r.lastContact.put(tag, LocalDateTime.now());
                }

                double temperature = Math.random() + 20.0;
                LocalDateTime dateTime = LocalDateTime.now().plusDays(i);

                r.writeTemperature(tag, temperature, dateTime);
                r.writeCurrentTemperature(tag, temperature, dateTime);
            }

        }*/


        /*try {
            FileWriter writer = new FileWriter("files/reader/readerConfig.csv", false);
            writer.write(csvRead.get(0).stream().collect(Collectors.joining(";")) + "\n");
            writer.write(csvConf.stream().collect(Collectors.joining(";")));
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

         */




    }
}
