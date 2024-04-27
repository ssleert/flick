import { Link } from "hywer/x/router";
import { routeNames } from "@/ui/routes";

import Icon from "@/ui/utils/Icon";
import FlickLogoSrc from "#shared/img/flick-logo.svg";
import MailIconSrc from "#shared/img/mail-icon.svg";
import LockIconSrc from "#shared/img/lock-icon.svg";

import css from "./Registration.module.less";

const Registration = () => {
  return (
    <div class={css.Page}>
      <div class={css.Registration}>
        <div class={css.Window}>
          <div class={css.Title}>
            <Icon aspect-ratio={[1, 1]} 
                  src={FlickLogoSrc} />
            <h3 class={css.Text}>
              Flick
            </h3>
          </div>

          <p class={css.Info}>
            Account Creation
          </p>

          <div class={css.InputFields}>
            <div class={css.Field}>
              <Icon aspect-ratio={[1, 1]} 
                    src={MailIconSrc} />
              <input type="text"
                     placeholder="nickname" />
            </div>

            <div class={css.Field}>
              <Icon aspect-ratio={[1, 1]} 
                    src={MailIconSrc} />
              <input type="text"
                     placeholder="example@mail.com" />
            </div>

            <div class={css.Field}>
              <Icon aspect-ratio={[1, 1]} 
                    src={LockIconSrc} />
              <input type="password"
                     placeholder="password" />
            </div>
          </div>


          <div class={css.LoginRedirectWrapper}>
            <Link path={routeNames.login} class={css.ToLoginText}>
              Already registered?
            </Link>
          </div>

          <button class={css.Button}>
            Register
          </button>
        </div>
      </div>
    </div>
  );
}

export default Registration;
