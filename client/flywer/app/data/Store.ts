import Auth from "@/data/auth/Auth";
import Api from "@/data/api/Api";

class Store {
  private auth = new Auth(Api);
};

const StoreInstance = new Store();
export default StoreInstance;
