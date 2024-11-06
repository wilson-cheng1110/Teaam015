import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { AppleIPadPro11_SilverLandscape } from './AppleIPadPro11_SilverLandscape/AppleIPadPro11_SilverLandscape.js';
import { Arrow_back } from './Arrow_back/Arrow_back.js';
import { Arrow_backIcon } from './Arrow_backIcon.js';
import classes from './Frame28.module.css';

interface Props {
  className?: string;
  hide?: {
    rectangle1?: boolean;
    group13?: boolean;
  };
}
/* @figmaId 207:306 */
export const Frame28: FC<Props> = memo(function Frame28(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <AppleIPadPro11_SilverLandscape
        className={classes.r}
        classes={{ appleIPadPro11_SilverLandscape: classes.appleIPadPro11_SilverLandscape }}
        hide={{
          rectangle1: true,
          group13: true,
        }}
      />
      <Arrow_back
        className={classes.arrow_back}
        swap={{
          icon: <Arrow_backIcon className={classes.icon} />,
        }}
      />
      <div className={classes.image3}></div>
      <div className={classes.cameraView}>Camera view</div>
    </div>
  );
});
