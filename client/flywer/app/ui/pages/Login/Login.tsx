import { ref } from "hywer";
import { Link, navigateTo } from "hywer/x/router";
import { routeNames } from "@/ui/routes";

import { addNotification } from "@/ui/widgets/Notifications/Notifications";
import requestNotificationWrapper from "@/ui/utils/requestNotificationWrapper";
import bindToRef from "@/ui/utils/bindToRef";

import Icon from "@/ui/utils/Icon";
import FlickLogoSrc from "#shared/img/flick-logo.svg";
import MailIconSrc from "#shared/img/mail-icon.svg";
import LockIconSrc from "#shared/img/lock-icon.svg";

import css from "./Login.module.less";

import store from "@/data/Store";

const Login = () => {
  const email = ref("");
  const password = ref("");

  const validateFields = () => {
    return (
      password.val.length > 5 &&
      password.val.length <= 2048 &&
      email.val.length > 5 &&
      email.val.length <= 2048
    );
  };

  const onFormSubmit = async (e: Event) => {
    e.preventDefault();

    if (!validateFields()) {
      addNotification("warn", "Form Validation", "Not all fields correct");
      return;
    }

    const resp = requestNotificationWrapper(
      await store.auth.login(email.val, password.val),
    );

    if (resp.error != "") {
      return;
    }

    addNotification("info", "Logged in", "Now u can enjoy content on Flick!");

    navigateTo(routeNames.root);
  };

  return (
    <div class={css.Page}>
      <div class={css.Login}>
        <div class={css.Window}>
          <div class={css.Title}>
            <Icon aspect-ratio={[1, 1]} src={FlickLogoSrc} />
            <h3 class={css.Text}>Flick</h3>
          </div>

          <p class={css.Info}>Authentication</p>

          <form onSubmit={onFormSubmit}>
            <div class={css.InputFields}>
              <div class={css.Field}>
                <Icon
                  aspect-ratio={[1, 1]}
                  src={MailIconSrc}
                  class={css.Icon}
                />
                <input
                  type="email"
                  placeholder="example@mail.com"
                  onInput={bindToRef(email)}
                />
              </div>

              <div class={css.Field}>
                <Icon
                  aspect-ratio={[1, 1]}
                  src={LockIconSrc}
                  class={css.Icon}
                />
                <input
                  type="password"
                  placeholder="password"
                  onInput={bindToRef(password)}
                />
              </div>
            </div>

            <div class={css.RegRedirectWrapper}>
              <Link path={routeNames.reg} class={css.ToLoginText}>
                Doesn't have an account?
              </Link>
            </div>

            <button class={css.Button} type="submit">
              Authenticate
            </button>
          </form>
        </div>
      </div>
    </div>
  );
};

export default Login;
