import Auth from "@/data/auth/Auth";
import Api from "@/data/api/Api";

class Store {
  public auth = new Auth(Api);
}

const StoreInstance = new Store();
export default StoreInstance;
