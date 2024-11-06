import { memo } from 'react';
import type { FC } from 'react';

import resets from '../../_resets.module.css';
import classes from './AppleIPadPro11_SilverLandscape.module.css';
import { Ellipse17Icon } from './Ellipse17Icon.js';
import { Ellipse18Icon } from './Ellipse18Icon.js';

interface Props {
  className?: string;
  classes?: {
    appleIPadPro11_SilverLandscape?: string;
    root?: string;
  };
  hide?: {
    rectangle1?: boolean;
    group13?: boolean;
  };
}
/* @figmaId 2:3 */
export const AppleIPadPro11_SilverLandscape: FC<Props> = memo(function AppleIPadPro11_SilverLandscape(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div
        className={`${props.classes?.appleIPadPro11_SilverLandscape || ''} ${classes.appleIPadPro11_SilverLandscape}`}
      ></div>
      {!props.hide?.rectangle1 && <div className={classes.rectangle1}></div>}
      <div className={classes.rECYCLABLE}>RECYCLABLE</div>
      <div className={classes.ellipse17}>
        <Ellipse17Icon className={classes.icon} />
      </div>
      <div className={classes.ellipse18}>
        <Ellipse18Icon className={classes.icon2} />
      </div>
    </div>
  );
});
