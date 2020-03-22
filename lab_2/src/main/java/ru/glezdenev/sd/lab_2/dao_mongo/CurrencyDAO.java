package ru.glezdenev.sd.lab_2.dao_mongo;

import ru.glezdenev.sd.lab_2.objects.Currecy;
import com.mongodb.client.model.Filters;
import com.mongodb.rx.client.MongoCollection;
import org.bson.Document;
import rx.Observable;

public class CurrencyDAO {

    public static Observable <Currecy> getCurrency () {
        return getAllCurrency(rxMongo.getCollection("currency"));
    }
    
    public static Observable <Currecy> findCurrency (String id) {
        return rxMongo.getCollection("currency").find(Filters.eq("_id", id)).toObservable().map(Currecy::new);
    }
    
    private static Observable <Currecy> getAllCurrency (MongoCollection <Document> collection) {
        return collection.find().toObservable().map(Currecy::new);
    }
}
