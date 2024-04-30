import { Api } from "@/data/api/Api";

class Auth<ApiImpl extends Api> {
  constructor(
    private api: ApiImpl,
  ) {};

  public async register(username: string, email: string, password: string): Promise<[number, string]> {
    // TODO: add input validation
    const resp = await this.api.registerUser(username, email, password);

    if (resp.error != "") {
      return [0, resp.error];
    }

    return [resp.userId, ""]
  };

  public async login(email: string, password: string): Promise<void> {
    return;
  };

  public async refresh(): Promise<void> {
    return;
  };
};

export default Auth;
