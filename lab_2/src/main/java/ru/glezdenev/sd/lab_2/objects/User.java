package ru.glezdenev.sd.lab_2.objects;

import java.util.Map;
import java.util.List;
import org.bson.Document;

public class User {
    public final int id;
    public final String name;
    public final String login;
    public final String currency;


    public User (Document doc) {
        this (
            doc.getDouble("_id").intValue(), 
            doc.getString("name"), 
            doc.getString("login"), 
            doc.getString("currency")
        );
    }

    public User (int id, String name, String login, String currency) {
        this.id = id;
        this.name = name;
        this.login = login;
        this.currency = currency;
    }
    
    public User (Map <String, List <String> > params) {
        this (
            Integer.parseInt(params.get("user_id").get(0)),
            params.get("user_name").get(0),
            params.get("user_login").get(0),
            params.get("user_currency").get(0)
        );
    }

    public String toString () {
        return "User{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", login='" + login + '\'' +
                ", currency='" + currency + '\'' +
                '}';
    }
    
    public Document toDocument ()
    {
        Document doc = new Document("_id", new Double(id))
            .append("name", name)
            .append("login", login)
            .append("currency", currency);
        return doc;
    }
}
