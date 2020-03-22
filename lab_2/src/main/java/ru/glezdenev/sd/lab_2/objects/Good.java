package ru.glezdenev.sd.lab_2.objects;

import java.util.Map;
import java.util.List;
import org.bson.Document;

public class Good {
    public final int id;
    public final String name;
    public int cnt;
    public double cost;


    public Good (Document doc) {
        this (
            doc.getDouble("_id").intValue(), 
            doc.getString("name"), 
            doc.getDouble("cnt").intValue(),
            doc.getDouble("cost")
        );
    }

    public Good (int id, String name, int cnt, double cost) {
        this.id = id;
        this.name = name;
        this.cnt = cnt;
        this.cost = cost;
    }
    
    public Good (Map <String, List <String> > params) {
        this ( 
            Integer.parseInt(params.get("object_id").get(0)),
            params.get("object_name").get(0),
            Integer.parseInt(params.get("object_cnt").get(0)),
            Double.parseDouble(params.get("object_cost").get(0))
        );
    }

    public String toString () {
        return toString(1.);
    }
    
    public String toString (double currency) {
        return "Object{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", cnt='" + cnt + '\'' +
                ", cost='" + (cost / currency) + '\'' +
                '}';
    }
    
    public Document toDocument () {
        Document doc = new Document("_id", new Double(id))
            .append("name", name)
            .append("cnt", new Double(cnt))
            .append("cost", new Double(cost));
        return doc;
    }
}

