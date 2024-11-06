import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import classes from './Arrow_back.module.css';
import { Arrow_backIcon } from './Arrow_backIcon.js';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
  swap?: {
    icon?: ReactNode;
  };
}
/* @figmaId 163:695 */
export const Arrow_back: FC<Props> = memo(function Arrow_back(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.icon}>{props.swap?.icon || <Arrow_backIcon className={classes.icon2} />}</div>
    </div>
  );
});
