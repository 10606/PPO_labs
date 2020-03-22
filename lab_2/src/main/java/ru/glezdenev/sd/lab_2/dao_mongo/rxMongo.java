package ru.glezdenev.sd.lab_2.dao_mongo;

import com.mongodb.rx.client.MongoClient;
import com.mongodb.rx.client.MongoClients;
import com.mongodb.rx.client.MongoCollection;
import org.bson.Document;
import rx.Observable;

public class rxMongo {

    private static MongoClient client = createMongoClient();
    
    public static Observable <com.mongodb.client.result.DeleteResult> clear (String name) {
        return getCollection(name).deleteMany(new Document());
    }

    static MongoCollection <Document> getCollection (String name) {
        return client.getDatabase("rxtest").getCollection(name);
    }

    private static MongoClient createMongoClient () {
        return MongoClients.create("mongodb://localhost:27017");
    }
}

