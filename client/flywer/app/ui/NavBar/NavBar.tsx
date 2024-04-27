import { Link } from "hywer/x/router";

import Icon from "@/ui/utils/Icon";
import MailIconSrc from "#shared/img/mail-icon.svg";
import SettingsIconSrc from "#shared/img/settings-icon.svg";
import SearchIconSrc from "#shared/img/search-icon.svg";
import PlusIconSrc from "#shared/img/plus-icon.svg";
import BubblesIconSrc from "#shared/img/bubbles-icon.svg";

import css from "./NavBar.module.less";

const NavBar = () => {
  return (
    <div class={css.NavBar}>
      <div class={css.Links}>
        <Link path="/" class={css.Route}>
          <Icon aspect-ratio={[1, 1]} 
                class={css.Icon}
                src={MailIconSrc} />
        </Link>
        <Link path="/messages" class={css.Route}>
          <Icon aspect-ratio={[1, 1]} 
                class={css.Icon}
                src={BubblesIconSrc} />
        </Link>
        <Link path="/new" class={css.Route}>
          <Icon aspect-ratio={[1, 1]} 
                class={css.Icon}
                src={PlusIconSrc} />
        </Link>
        <Link path="/search" class={css.Route}>
          <Icon aspect-ratio={[1, 1]} 
                class={css.Icon}
                src={SearchIconSrc} />
        </Link>
        <Link path="/settings" class={css.Route}>
          <Icon aspect-ratio={[1, 1]} 
                class={css.Icon}
                src={SettingsIconSrc} />
        </Link>
      </div>
    </div>
  )
}

export default NavBar;
