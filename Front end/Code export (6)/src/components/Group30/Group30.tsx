import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { AppleIPadPro11_SilverLandscape } from './AppleIPadPro11_SilverLandscape/AppleIPadPro11_SilverLandscape.js';
import classes from './Group30.module.css';

interface Props {
  className?: string;
  hide?: {
    rectangle1?: boolean;
    group13?: boolean;
  };
}
/* @figmaId 176:250 */
export const Group30: FC<Props> = memo(function Group30(props = {}) {
  return (
    <>
      <AppleIPadPro11_SilverLandscape
        className={classes.appleIPadPro11_SilverLandscape}
        classes={{ appleIPadPro11_SilverLandscape: classes.a }}
        hide={{
          rectangle1: true,
          group13: true,
        }}
      />
      <div className={classes.image4}></div>
      <div className={classes.unnamed}>􀋚</div>
      <div className={classes.unnamed2}>􀌇</div>
      <div className={classes.unnamed3}>􀛨</div>
      <div className={classes.unnamed4}>􀙇</div>
      <div className={classes._100}>100%</div>
      <div className={classes._1019}>10:19</div>
      <div className={classes.wed6Nov}>Wed 6 Nov</div>
    </>
  );
});
