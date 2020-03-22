package ru.glezdenev.sd.lab_2.server;

import ru.glezdenev.sd.lab_2.dao_mongo.GoodsDAO;
import ru.glezdenev.sd.lab_2.dao_mongo.UserDAO;
import ru.glezdenev.sd.lab_2.dao_mongo.CurrencyDAO;
import ru.glezdenev.sd.lab_2.objects.User;
import ru.glezdenev.sd.lab_2.objects.Good;
import rx.Observable;
import java.util.Map;
import java.util.List;

public class GoodHandler {

    static Observable <String> getObject (Map <String, List <String> > params) {
        int user_id = Integer.parseInt(params.get("user_id").get(0));
        
        Observable <String> response = 
            UserDAO.findUser(user_id)
            .flatMap (
                user -> CurrencyDAO.findCurrency(user.currency)
                .flatMap (
                    currency -> {   
                        return GoodsDAO.getObject()
                        .map(object -> object.toString(currency.value) + "\n");
                    }
                )
            );
        return response;
    }
    
    static Observable <String> addObject (Map <String, List <String> > params) {
        int user_id = Integer.parseInt(params.get("user_id").get(0));
        
        Good newObject = new Good(params);
        Observable <String> response = 
            UserDAO.findUser(user_id)
            .flatMap (
                user -> CurrencyDAO.findCurrency(user.currency)
                .flatMap (
                    currency ->  {   
                        newObject.cost *= currency.value;
                        return GoodsDAO.addObject(newObject)
                        .map(usd -> "ok");
                    }
                )
            );
        return response;
    }
}
