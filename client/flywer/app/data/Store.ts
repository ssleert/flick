import Auth from "@/data/auth/Auth";

class Store {
  private auth = new Auth(); 
};

const StoreState = new Store();
export default StoreState;
