import { Api, RegisterUserResponse, LoginUserResponse } from "@/data/api/Api";

class Auth<ApiImpl extends Api> {
  constructor(private api: ApiImpl) {}

  public accessToken: string = "";
  public refreshToken: string = "";

  public async register(
    username: string,
    email: string,
    password: string,
  ): Promise<RegisterUserResponse> {
    // TODO: add input validation
    const resp = await this.api.registerUser(username, email, password);

    return resp;
  }

  public async login(
    email: string,
    password: string,
  ): Promise<LoginUserResponse> {
    const resp = await this.api.loginUser(email, password);

    if (resp.error != "") {
      return resp;
    }

    this.accessToken = resp.accessToken;
    this.refreshToken = resp.refreshToken;

    return resp;
  }

  public async refresh(): Promise<void> {
    return;
  }
}

export default Auth;
