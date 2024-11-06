import { memo } from 'react';
import type { FC, ReactNode } from 'react';

import resets from '../../_resets.module.css';
import { Edit } from '../Edit/Edit.js';
import classes from './FAB_StyleSecondaryStateEnabled.module.css';
import { Icon } from './Icon.js';

interface Props {
  className?: string;
  classes?: {
    root?: string;
  };
  swap?: {
    icon?: ReactNode;
  };
}
/* @figmaId 158:814 */
export const FAB_StyleSecondaryStateEnabled: FC<Props> = memo(function FAB_StyleSecondaryStateEnabled(props = {}) {
  return (
    <div className={`${resets.clapyResets} ${props.classes?.root || ''} ${props.className || ''} ${classes.root}`}>
      <div className={classes.stateLayer}>
        {props.swap?.icon || (
          <Edit
            swap={{
              icon: <Icon className={classes.icon} />,
            }}
          />
        )}
      </div>
    </div>
  );
});
