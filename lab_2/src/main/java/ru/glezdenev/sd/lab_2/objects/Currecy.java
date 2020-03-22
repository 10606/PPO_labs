package ru.glezdenev.sd.lab_2.objects;

import java.util.Map;
import java.util.List;
import org.bson.Document;

public class Currecy {
    public final String id;
    public final double value;


    public Currecy (Document doc) {
        this (
            doc.getString("_id"), 
            doc.getDouble("value")
        );
    }

    public Currecy (String id, double value) {
        this.id = id;
        this.value = value;
    }

    public String toString () {
        return "Currecy{" +
                "_id='" + id + '\'' +
                ", value='" + value + '\'' +
                '}';
    }
    
    public Document toDocument () {
        Document doc = new Document("_id", id)
            .append("value", new Double(value));
        return doc;
    }
}


