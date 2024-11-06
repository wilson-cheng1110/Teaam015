import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import classes from './Add.module.css';
import { AddIcon } from './AddIcon.js';

interface Props {
  className?: string;
  swap?: {
    icon?: ReactNode;
  };
}
/* @figmaId 158:778 */
export const Add: FC<Props> = memo(function Add(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${classes.root}`}>
      <div className={classes.icon}>{props.swap?.icon || <AddIcon className={classes.icon2} />}</div>
    </div>
  );
});
