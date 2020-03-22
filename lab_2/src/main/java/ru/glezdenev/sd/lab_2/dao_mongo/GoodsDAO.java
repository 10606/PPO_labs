package ru.glezdenev.sd.lab_2.dao_mongo;

import ru.glezdenev.sd.lab_2.objects.Good;
import com.mongodb.rx.client.MongoCollection;
import org.bson.Document;
import rx.Observable;

public class GoodsDAO {
    
    public static Observable <Good> getObject () {
        return getAllObjects(rxMongo.getCollection("object"));
    }
    
    public static Observable <com.mongodb.rx.client.Success> addObject (Good value) {
        return rxMongo.getCollection("object").insertOne(value.toDocument());
    }
    
    private static Observable <Good> getAllObjects (MongoCollection <Document> collection) {
        return collection.find().toObservable().map(Good::new);
    }

}
