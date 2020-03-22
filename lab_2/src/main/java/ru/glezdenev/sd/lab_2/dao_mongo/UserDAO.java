package ru.glezdenev.sd.lab_2.dao_mongo;

import ru.glezdenev.sd.lab_2.objects.User;
import com.mongodb.client.model.Filters;
import com.mongodb.rx.client.MongoCollection;
import org.bson.Document;
import rx.Observable;

public class UserDAO {

    public static Observable <User> getUser () {
        return getAllUsers(rxMongo.getCollection("user"));
    }
    
    public static Observable <User> findUser (int id) {
        return rxMongo.getCollection("user").find(Filters.eq("_id", id)).toObservable().map(User::new);
    }
    
    public static Observable <com.mongodb.rx.client.Success> addUser (User value) {
        return rxMongo.getCollection("user").insertOne(value.toDocument());
    }
    
    private static Observable <User> getAllUsers (MongoCollection <Document> collection) {
        return collection.find().toObservable().map(User::new);
    }
}
