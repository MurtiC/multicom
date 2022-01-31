package com.company;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.SQLOutput;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Logger {
    FileWriter writer;

    public Logger() {
        File dir = new File("files/reader/logs");
        if (!dir.exists()) {
            dir.mkdir();
        }
    }

    public void log(String log) {
        try {
            System.out.println(log);
            writer = new FileWriter("files/reader/logs/log-" + LocalDateTime.now().toLocalDate()+".txt", true);
            writer.append(LocalDateTime.now().toLocalTime().format(DateTimeFormatter.ofPattern("HH:mm:ss")) + ": " + log + "\n");
            writer.close();
            this.deleteOldestFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void deleteOldestFile(){
        File f1 = new File("files/logs/log-" + LocalDateTime.now().toLocalDate().minusDays(31) + ".txt");
        if(f1.exists()) {
            f1.delete();
        }
    }


}
