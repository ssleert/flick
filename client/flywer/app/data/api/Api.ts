import Constants from "@/data/Constants";
import { unpack as msgpackUnpack } from 'msgpackr/unpack';

export interface ApiResponse {
  error: string;
};

export class RegisterUserResponse {
  constructor(
    public error: string = "",
    public userId: number = 0,
  ) {};
};

export class LoginUserResponse {
  constructor(
    public error: string = "",
    public accessToken: string = "",
    public refreshToken: string = "",
  ) {};
};

export class Api {
  public static readonly RequestError = "request failed";
  public static isError(resp: ApiResponse): boolean {
    return resp.error != "";
  }

  private async getResponse(method: string, path: string): Promise<ApiResponse> {
    try {
      const resp = await fetch(Constants.apiUrl + path, {
        method: method,
      });
      const body = await msgpackUnpack(await resp.arrayBuffer());
      if (body["error"] === undefined) {
        return {error: Api.RequestError};
      }
      return body;
    } catch (err) {
      console.log(err);
      return {error: Api.RequestError};
    }
  }

  public async registerUser(username: string, email: string, password: string): Promise<RegisterUserResponse> {
    const resp = await this.getResponse("POST", `/v1/register_user?email=${email}&username=${username}&password=${password}`);
    if (resp.error != "") {
      return new RegisterUserResponse(resp.error);
    }

    return new RegisterUserResponse("", (resp as any)["user_id"] ?? 0);
  }

  public async loginUser(email: string, password: string): Promise<LoginUserResponse> { 
    const resp = await this.getResponse("POST", `/v1/login_user?email=${email}&password=${password}`);
    if (resp.error != "") {
      return new LoginUserResponse(resp.error)
    }

    const data = (resp as any);

    return new LoginUserResponse(
      resp.error, 
      data["access_token"], 
      data["refresh_token"]
    );
  }
};

const ApiInstance = new Api();
export default ApiInstance;
