import { Api } from "@/data/api/Api";
import { addNotification } from "@/ui/widgets/Notifications/Notifications";

interface ResponseWithError {
  error: string;
};

const requestNotificationWrapper = <T extends ResponseWithError>(obj: T): T => {
  if (obj.error == Api.RequestError) {
    addNotification(
      "bad", 
      "Request Error", 
      "Request to backend failed. Check network connection or browser console.",
    );

    return obj;
  }

  if (obj.error != "") { 
    addNotification(
      "bad", 
      "Backend Error", 
      "Internal Backend Error: " + obj.error,
    );

    return obj;
  }

  return obj;
}

export default requestNotificationWrapper;
