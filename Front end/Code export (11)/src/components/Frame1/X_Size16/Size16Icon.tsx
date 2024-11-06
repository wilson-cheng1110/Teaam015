import { memo, SVGProps } from 'react';

const Size16Icon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 8 8' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path d='M8 0L0 8M0 0L8 8' stroke='#1E1E1E' strokeWidth={1.6} strokeLinecap='round' strokeLinejoin='round' />
  </svg>
);

const Memo = memo(Size16Icon);
export { Memo as Size16Icon };
