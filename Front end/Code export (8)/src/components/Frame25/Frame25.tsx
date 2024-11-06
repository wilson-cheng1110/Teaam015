import { memo } from 'react';
import type { FC } from 'react';

import resets from '../_resets.module.css';
import { Ellipse17Icon } from './Ellipse17Icon.js';
import { Ellipse18Icon } from './Ellipse18Icon.js';
import classes from './Frame25.module.css';
import { Group15Icon } from './Group15Icon.js';
import { Rectangle2Icon } from './Rectangle2Icon.js';

interface Props {
  className?: string;
}
/* @figmaId 207:298 */
export const Frame25: FC<Props> = memo(function Frame25(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.rECYCLABLE}>RECYCLABLE</div>
      <div className={classes.ellipse17}>
        <Ellipse17Icon className={classes.icon} />
      </div>
      <div className={classes.ellipse18}>
        <Ellipse18Icon className={classes.icon2} />
      </div>
      <div className={classes.unnamed}>􀋚</div>
      <div className={classes.unnamed2}>􀌇</div>
      <div className={classes.unnamed3}>􀛨</div>
      <div className={classes.unnamed4}>􀙇</div>
      <div className={classes._100}>100%</div>
      <div className={classes._1019}>10:19</div>
      <div className={classes.wed6Nov}>Wed 6 Nov</div>
      <div className={classes.unnamed5}>
        <div className={classes.unnamed6}>􀸺</div>
        <div className={classes.fOOD}>FOOD</div>
        <div className={classes.group15}>
          <Group15Icon className={classes.icon3} />
        </div>
      </div>
      <div className={classes.unnamed7}>􀙛</div>
      <div className={classes.frame24}>
        <div className={classes.rectangle2}>
          <Rectangle2Icon className={classes.icon4} />
        </div>
        <div className={classes.frame6}>
          <div className={classes.unnamed8}>􀑓</div>
          <div className={classes.pLANEDETAILS}>PLANE DETAILS</div>
        </div>
        <div className={classes.frame8}>
          <div className={classes.unnamed9}>􀣋</div>
          <div className={classes.sETTINGS}>SETTINGS</div>
        </div>
        <div className={classes.hELP}>HELP</div>
        <div className={classes.unnamed10}>􀿨</div>
        <div className={classes.cARTCAMS}>CART CAMS</div>
        <div className={classes.unnamed11}>􀎼</div>
      </div>
      <div className={classes.unnamed12}>􀆄</div>
      <div className={classes.unnamed13}>􀛨</div>
      <div className={classes.unnamed14}>􀙇</div>
      <div className={classes._1002}>100%</div>
      <div className={classes.rectangle3}></div>
    </div>
  );
});
