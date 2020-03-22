package ru.glezdenev.sd.lab_2.server;

import ru.glezdenev.sd.lab_2.dao_mongo.UserDAO;
import ru.glezdenev.sd.lab_2.dao_mongo.CurrencyDAO;
import ru.glezdenev.sd.lab_2.objects.User;
import rx.Observable;
import java.util.Map;
import java.util.List;

public class UserHandler {
    
    static Observable <String> addUser (Map <String, List <String> > params) {
        User user = new User(params);
        Observable <String> response = 
            CurrencyDAO.findCurrency(user.currency)
            .flatMap (
                curr -> UserDAO.addUser(user)
                .map(usd -> "ok")
            );
        return response;
    }

    static Observable <String> listUser (Map <String, List <String> > params) {
        Observable <String> response = 
            UserDAO.getUser()
                .map(usd -> usd.toString() + "\n");

        return response;
    }
}
