package ru.glezdenev.sd.lab_2.server;

import java.lang.InterruptedException;
import io.reactivex.netty.protocol.http.server.HttpServer;
import rx.Observable;
import java.util.Map;
import java.util.List;

public class rxServer {

    public static void main (final String[] args) throws InterruptedException {
        HttpServer
            .newServer(8080)
            .start((req, resp) -> {
            try {
                Map <String, List <String> > params = req.getQueryParameters();
                String name = req.getDecodedPath().substring(1);
                return resp.writeString(handle(name, params));
            }
            catch (NullPointerException e) {
                return resp.writeString(Observable.just("error (has not value)"));
            }
            catch (NumberFormatException e) {
                return resp.writeString(Observable.just("error (expected number) " + e.toString()));
            }
        })
        .awaitShutdown();
    }
    
    private static Observable <String> handle (String name, Map <String, List <String> > params) {
        if (name.equals("addUser")) {
            return UserHandler.addUser(params);
        }
        else if (name.equals("users")) {
            return UserHandler.listUser(params);
        }
        else if (name.equals("getObject")) {
            return GoodHandler.getObject(params);
        }
        else if (name.equals("addObject")) {
            return GoodHandler.addObject(params);
        }
        else if (name.equals("clear")) {
            return OtherHandler.clear(params);
        }
        else if (name.equals("currency")) {
            return OtherHandler.listCurrency(params);
        }
        else {
            return Observable.just("unknown command");
        }
    }
}
