import css from "./Notifications.module.less";

const notificationsListElement = <div class={css.NotificationsList}></div>;

type NotificationStatus = "info" | "good" | "warn" | "bad";
interface NotificationProps {
  header: string;
  body: string;
  status: NotificationStatus;
};

const cssNotifStatus = {
  "info": css.StatusInfo,
  "good": css.StatusGood,
  "warn": css.StatusWarn,
  "bad":  css.StatusBad,
};

const Notification = ({ header, body, status }: NotificationProps) => {
  return (
    <div class={css.NotificationContainer}>
      <button 
        class={css.CloseButton}
        onClick={(e: Event) => (e.target as HTMLElement)?.parentElement?.remove()}
      >
        X {/* TODO: add icon */}
      </button>

      <div class={css.Notification}>
        <div class={css.Header}>
          <div class={`${css.Status} ${cssNotifStatus[status]}`}></div>
          <div class={css.Text}>
            {header}
          </div>
        </div>

        <div class={css.Content}>
          {body}
        </div>
      </div>
    </div>
  );
}

export const addNotification = (
  status: NotificationStatus,
  header: string,
  body: string,
) => {
  const notifElement = <Notification
    status={status} 
    header={header} 
    body={body} 
  />;

  setTimeout(() => {
    notifElement?.remove();
  }, 8000);

  notificationsListElement.append(notifElement);
}

const NotificationsList = () => {
  return notificationsListElement;
}

export default NotificationsList;
