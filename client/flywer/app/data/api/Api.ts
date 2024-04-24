import Constants from "@/data/Constants";
import { decodeAsync as msgpackDecodeAsync } from "@msgpack/msgpack" 

export interface ApiResponse {
  error: string;
};

export class RegisterUserResponse {
  constructor(
    public error: string = "",
    public userId: number = 0,
  ) {};
};

class Api {
  public static readonly RequestError = "request failed";
  public static isError(resp: ApiResponse): boolean {
    return resp.error != "";
  }

  private async getResponse(method: string, path: string): Promise<ApiResponse> {
    try {
      const resp = await fetch(Constants.apiUrl + path, {
        method: method,
      });
      const body = await msgpackDecodeAsync(resp.body as any) as any;
      if (body["error"] === undefined) {
        return {error: Api.RequestError};
      }
      return body;
    } catch (err) {
      return {error: Api.RequestError};
    }
  }

  public async registerUser(email: string, username: string, password: string): Promise<RegisterUserResponse> {
    const resp = await this.getResponse("POST", `/v1/register_user?email=${email}&username=${username}&password=${password}`);
    if (resp.error != "") {
      return new RegisterUserResponse(resp.error);
    }

    return new RegisterUserResponse(resp.error, (resp as any)["user_id"] ?? 0);
  }
};

const ApiInstance = new Api();
export default ApiInstance;
