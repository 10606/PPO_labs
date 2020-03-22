package ru.glezdenev.sd.lab_2.server;

import ru.glezdenev.sd.lab_2.dao_mongo.rxMongo;
import ru.glezdenev.sd.lab_2.dao_mongo.CurrencyDAO;
import rx.Observable;
import java.util.Map;
import java.util.List;

public class OtherHandler {
    
    static Observable <String> listCurrency (Map <String, List <String> > params) {
        Observable <String> response = 
            CurrencyDAO.getCurrency()
                .map(usd -> usd.toString()  + "\n");

        return response;
    }
    
    static Observable <String> clear (Map <String, List <String> > params) {
        Observable <String> response = 
            rxMongo.clear("user")
            .flatMap (
                unused ->
                rxMongo.clear("object")
                .map(a -> "ok")
            );
        return response;
    }
}
